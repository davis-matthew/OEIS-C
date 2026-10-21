/*
 * A065562  a(n) = b(n)-th highest positive integer not equal to any a(k), 1
 *          <= k < n, where {b(n)} = 1, 1, 2, 1, 2, 3, 1, 2, 3, 4, 1, 2, 3, 4,
 *          5, ... (sequence A002260).
 * https://oeis.org/A065562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the b(n)-th smallest positive integer not already used,
 *            where b(n) is A002260 (1,1,2,1,2,3, ...). The function caches
 *            computed terms and the set of values already used. It works for
 *            any unsigned integer type seq_t and idx_t
 * Range:     u8 n <= 243, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/* ----- static state ---------------------------------------------------- */
static seq_t *a_seq   = NULL;  /* 1‑based cache: a_seq[0] unused */
static size_t a_cap   = 0;
static size_t a_len   = 0;     /* number of stored terms (including dummy at 0) */

static unsigned char *used   = NULL;  /* used[v] == 1 if v has appeared as a(k) */
static size_t         used_cap = 0;
static idx_t          next_free = 1;   /* smallest value not yet used */

/* ----- helper: grow the used table ------------------------------------- */
static void ensure_used_capacity(idx_t needed)
{
    if ((size_t)needed >= used_cap) {
        size_t new_cap = used_cap ? used_cap * 2 : 2;
        while (new_cap <= (size_t)needed) new_cap <<= 1;
        unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
        if (!tmp) exit(1);   /* out‑of‑memory abort */
        /* initialise the newly added part to zero */
        for (size_t i = used_cap; i < new_cap; ++i) tmp[i] = 0;
        used = tmp;
        used_cap = new_cap;
    }
}

/* ----- helper: grow the a‑sequence table -------------------------------- */
static void ensure_a_capacity(idx_t needed)
{
    if ((size_t)needed >= a_cap) {
        size_t new_cap = a_cap ? a_cap * 2 : 1;
        while (new_cap <= (size_t)needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_seq, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        a_seq = tmp;
        a_cap = new_cap;
    }
}

/* ----- compute b(n) ----------------------------------------------------- */
static inline idx_t compute_b(idx_t n)
{
    /* Find the smallest m such that m*(m+1)/2 >= n */
    double d = sqrt((double)(8ull * n + 1));
    idx_t m = (idx_t)((d - 1.0) / 2.0);
    while (((unsigned __int128)m * (m + 1)) / 2 < n) ++m;
    while (m > 0 && ((unsigned __int128)m * (m - 1)) / 2 >= n) --m;
    idx_t cumulative_prev = ((unsigned __int128)m * (m - 1)) / 2;
    return n - cumulative_prev;
}

/* ----- find the b‑th unused integer and mark it ------------------------ */
static seq_t pick_bth_unused(idx_t b)
{
    idx_t count = 0;
    idx_t cand  = next_free;

    while (1) {
        ensure_used_capacity(cand);
        if (!used[cand]) {          /* value not yet used */
            ++count;
            if (count == b) break;
        }
        ++cand;
    }

    /* mark chosen candidate as used */
    used[cand] = 1;

    /* advance next_free past the chosen number if needed */
    if (cand == next_free) {
        idx_t nf = cand + 1;
        while (1) {
            ensure_used_capacity(nf);
            if (!used[nf]) break;
            ++nf;
        }
        next_free = nf;
    }

    return (seq_t)cand;
}

/* ----- public sequence function ---------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence defined for n >= 1 */

    /* initialise on first call */
    if (!a_seq) {
        a_cap = 1; a_len = 1;          /* a_seq[0] dummy sentinel */
        a_seq = (seq_t *)realloc(NULL, a_cap * sizeof(seq_t));
        a_seq[0] = 0;

        used_cap = 2;                 /* indices 0 and 1 (value 1 will be used) */
        used    = (unsigned char *)realloc(NULL, used_cap);
        used[0] = 0; used[1] = 0;
        next_free = 1;
    }

    /* compute terms up to n if needed */
    while (a_len <= (size_t)n) {
        idx_t idx = a_len;           /* next position to compute */
        idx_t b   = compute_b(idx);
        seq_t val = pick_bth_unused(b);

        ensure_a_capacity(idx);
        a_seq[idx] = val;
        ++a_len;
    }

    return a_seq[(size_t)n];
}
