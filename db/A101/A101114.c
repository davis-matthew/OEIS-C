/*
 * A101114  Let t(G) = number of unitary factors of the Abelian group G. Then
 *          a(n) = sum t(G) over all Abelian groups G of order <= n.
 * https://oeis.org/A101114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{m<=n} (product over prime powers p^e||m of p(e)) *
 *            2^{ω(m)}, where p(e) is the number of integer partitions of e
 *            and ω(m) is the number of distinct prime divisors of m. This
 *            equals sum over all Abelian groups G of order <= n of the number
 *            of unitary factors of G
 * Range:     u8 n <= 48, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* 10‑partition table: part[e] gives the number of integer partitions of e
   (part[0] unused).  Exponents up to 13 are sufficient for m <= 10000.  */
static const seq_t part[14] = {0,
                               1, /* 1 */
                               2, /* 2 */
                               3, /* 3 */
                               5, /* 4 */
                               7, /* 5 */
                               11,/* 6 */
                               15,/* 7 */
                               22,/* 8 */
                               30,/* 9 */
                               42,/*10*/
                               56,/*11*/
                               77,/*12*/
                               101/*13*/};

/*  Dynamic storage for small helper tables */
static int   *spf = NULL;      /* smallest prime factor */
static int   *omega = NULL;    /* count of distinct primes */
static seq_t *cum = NULL;      /* cumulative sums a(m) */
static size_t capacity = 0;    /* current size of the above arrays */

/*  Ensure tables are allocated and filled up to new_cap-1 (n+1)  */
static void extend_tables(size_t new_cap)
{
    if (new_cap <= capacity) return;

    /* Reallocate memory for the tables */
    spf   = (int*)   realloc(spf,   new_cap * sizeof(int));
    omega = (int*)   realloc(omega, new_cap * sizeof(int));
    cum   = (seq_t*) realloc(cum,   new_cap * sizeof(seq_t));

    /* Initialise spf array */
    for (size_t i = 0; i < new_cap; ++i) spf[i] = 0;
    if (new_cap > 0) spf[0] = 0;
    if (new_cap > 1) spf[1] = 1;

    /* Sieve for smallest prime factor */
    for (size_t i = 2; i < new_cap; ++i) {
        if (spf[i] == 0) {
            spf[i] = (int)i;
            if (i * i < new_cap) {
                for (size_t j = i * i; j < new_cap; j += i)
                    if (spf[j] == 0) spf[j] = (int)i;
            }
        }
    }

    /* Compute omega (distinct prime count) for each number */
    if (new_cap > 0) omega[0] = 0;
    if (new_cap > 1) omega[1] = 0;
    for (size_t i = 2; i < new_cap; ++i) {
        int x = (int)i;
        int prev = 0, cnt = 0;
        while (x > 1) {
            int p = spf[x];
            if (p != prev) { cnt++; prev = p; }
            while (x % p == 0) x /= p;
        }
        omega[i] = cnt;
    }

    /* Compute cumulative sums a(m) for new range */
    if (capacity == 0) cum[0] = 0;          /* base case a(0) = 0 */
    size_t start = capacity;
    if (start == 0) start = 1;             /* first real term is for m = 1 */

    for (size_t m = start; m < new_cap; ++m) {
        seq_t fm = 1;
        int x = (int)m;
        while (x > 1) {
            int p = spf[x];
            int e = 0;
            while (x % p == 0) { x /= p; ++e; }
            fm *= part[e];
        }
        seq_t contrib = fm * ((seq_t)1 << omega[m]);
        cum[m] = cum[m-1] + contrib;
    }

    capacity = new_cap;
}

/*  Public function required by the problem statement  */
seq_t a(idx_t n)
{
    size_t nn = (size_t)n;          /* convert to native size_t for indexing */
    if (nn + 1 > capacity) extend_tables(nn + 1);
    return cum[nn];
}
