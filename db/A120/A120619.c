/*
 * A120619  a(n) = b(n)th integer among those positive integers not yet
 *          occurring in {a(k)}, where {b(k)} is the concatenation of the
 *          divisors of the positive integers.
 * https://oeis.org/A120619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the b(n)-th unused positive integer, where b(n) =
 *            A027750(n) – the concatenation of the divisors of the positive
 *            integers. b(k) is generated incrementally; a(n) is computed by
 *            maintaining a Fenwick tree of unused numbers
 * Range:     u8 n <= 206, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* ----- Fenwick tree for k‑th unused integer ----- */
static unsigned *fenwick = NULL;      /* 1‑based */
static size_t     fenwick_n = 0;     /* size of the tree */

/* ensure the Fenwick tree has at least `need` positions (1‑based) */
static void ensure_fenwick_size(idx_t need)
{
    if ((idx_t)fenwick_n >= need)
        return;
    size_t new_n = fenwick_n ? fenwick_n << 1 : 1;
    while (new_n < (size_t)need)
        new_n <<= 1;
    fenwick = (unsigned*)realloc(fenwick, (new_n + 1) * sizeof(unsigned));
    if (!fenwick)
        exit(1);
    /* initialise new slots: all positions initially unused */
    for (size_t i = fenwick_n + 1; i <= new_n; ++i)
        fenwick[i] = (unsigned)(i & -i);
    fenwick_n = new_n;
}

/* add `delta` to position `idx` (1‑based) */
static void fenwick_add(size_t idx, int delta)
{
    while (idx <= fenwick_n)
    {
        fenwick[idx] += delta;
        idx += idx & -idx;
    }
}

/* find the smallest index such that the number of unused integers up
   to that index is at least `k`.  `k` is 1‑based. */
static size_t fenwick_kth(idx_t k)
{
    size_t idx = 0;
    /* get highest power of two <= fenwick_n */
    size_t bit = 1;
    while (bit < fenwick_n)
        bit <<= 1;
    while (bit)
    {
        size_t next = idx + bit;
        if (next <= fenwick_n && fenwick[next] < (unsigned)k)
        {
            idx = next;
            k -= fenwick[next];
        }
        bit >>= 1;
    }
    return idx + 1;          /* 1‑based position */
}

/* ----- Global data for the sequence ----- */
static seq_t *a_seq   = NULL;  /* array of a(k) */
static size_t a_cap   = 0;     /* allocated capacity of a_seq */
static idx_t  a_used  = 0;     /* number of a terms already computed */

static seq_t *b_seq   = NULL;  /* array of b(k) values */
static size_t b_cap   = 0;     /* capacity of b_seq */
static idx_t  b_used  = 0;     /* number of b terms already generated */
static idx_t  m_next  = 1;     /* next integer whose divisors are to be listed */

/* expand the a array if necessary */
static void ensure_a_capacity(size_t min_cap)
{
    if (a_cap >= min_cap)
        return;
    size_t new_cap = a_cap ? a_cap : 8;
    while (new_cap < min_cap)
        new_cap <<= 1;
    a_seq = (seq_t*)realloc(a_seq, new_cap * sizeof(seq_t));
    if (!a_seq)
        exit(1);
    a_cap = new_cap;
}

/* expand the b array if necessary */
static void ensure_b_capacity(size_t min_cap)
{
    if (b_cap >= min_cap)
        return;
    size_t new_cap = b_cap ? b_cap : 8;
    while (new_cap < min_cap)
        new_cap <<= 1;
    b_seq = (seq_t*)realloc(b_seq, new_cap * sizeof(seq_t));
    if (!b_seq)
        exit(1);
    b_cap = new_cap;
}

/* generate b terms until we have at least `target` of them */
static void generate_b_up_to(idx_t target)
{
    while (b_used < target)
    {
        idx_t m = m_next;
        /* collect divisors of m */
        idx_t small[512], large[512];
        size_t sc = 0, lc = 0;
        for (idx_t d = 1; d <= m / d; ++d)
        {
            if (m % d == 0)
            {
                if (sc < sizeof(small)/sizeof(small[0]))
                    small[sc++] = d;
                if (d != m / d)
                {
                    if (lc < sizeof(large)/sizeof(large[0]))
                        large[lc++] = m / d;
                }
            }
        }
        /* push small divisors in ascending order */
        for (size_t i = 0; i < sc; ++i)
        {
            if (b_used == b_cap)
                ensure_b_capacity(b_cap + 8);
            b_seq[b_used++] = (seq_t)small[i];
        }
        /* push large divisors in ascending order (reverse of collected) */
        for (size_t i = 0; i < lc; ++i)
        {
            if (b_used == b_cap)
                ensure_b_capacity(b_cap + 8);
            b_seq[b_used++] = (seq_t)large[lc - 1 - i];
        }
        ++m_next;
    }
}

/* ------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined for n=0, but safe */

    /* initialise Fenwick tree if not yet done */
    if (!fenwick)
        ensure_fenwick_size(262144u);     /* start with 2^18 positions */

    /* make sure we have enough b values */
    if (b_used < n)
        generate_b_up_to(n);

    /* initialise a_seq if empty */
    if (!a_seq)
    {
        a_cap = 8;
        a_seq = (seq_t*)calloc(a_cap, sizeof(seq_t));
        if (!a_seq)
            exit(1);
    }

    /* compute missing a terms up to n */
    while (a_used < n)
    {
        seq_t b_val = b_seq[a_used];    /* b(a_used+1) */
        idx_t k = (idx_t)b_val;

        /* Ensure the Fenwick tree has enough unused integers to satisfy k */
        size_t needed = (size_t)k + (size_t)a_used + 100;
        if (fenwick_n < needed)
            ensure_fenwick_size(needed);

        size_t idx_val = fenwick_kth(k);
        if (a_used == a_cap)
            ensure_a_capacity(a_cap + 8);
        a_seq[a_used] = (seq_t)idx_val;
        fenwick_add(idx_val, -1);   /* mark as used */
        ++a_used;
    }

    return a_seq[n - 1];
}
