/*
 * A116816  Number of permutations of length n which avoid the patterns 2314,
 *          3124, 4312.
 * https://oeis.org/A116816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A116816 – number of permutations avoiding 2314, 3124, 4312
   Recurrence (derived from the rational generating function):
   a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=75, a(6)=260
   a(n) = 8*a(n‑1) - 24*a(n‑2) + 35*a(n‑3) - 28*a(n‑4) + 12*a(n‑5) - 2*a(n‑6)   (n ≥ 7)
   The function memoises previously computed values so that calls may come in any order.
*/

#include <stdlib.h>

/* initial six values */
static const seq_t INIT[6] = {
    (seq_t)1, (seq_t)2, (seq_t)6, (seq_t)21, (seq_t)75, (seq_t)260
};

/* cache for memoisation */
static seq_t *CACHE = NULL;          /* array of computed a(n) values (index n-1) */
static size_t CACHE_LEN = 0;         /* number of values currently stored (≥0) */
static size_t CACHE_CAP = 0;         /* current allocation capacity */

/* grow the cache to hold at least 'needed' elements */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= CACHE_CAP)
        return;
    size_t new_cap = CACHE_CAP ? CACHE_CAP * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    CACHE = (seq_t *)realloc(CACHE, new_cap * sizeof(seq_t));
    CACHE_CAP = new_cap;
}

seq_t a(idx_t n)
{
    if (n <= 0)
        return (seq_t)0;                           /* undefined for n ≤ 0 */

    /* quick return for the first six terms */
    if (n <= 6)
        return INIT[n - 1];

    /* initialise cache with the first six values on first call */
    if (CACHE_LEN == 0) {
        ensure_cache_cap(6);
        for (size_t i = 0; i < 6; ++i)
            CACHE[i] = INIT[i];
        CACHE_LEN = 6;
    }

    /* compute any missing values up to n */
    if ((size_t)n > CACHE_LEN) {
        ensure_cache_cap((size_t)n);
        for (idx_t k = CACHE_LEN + 1; k <= n; ++k) {
            /* indices in CACHE:
               a(k-1) -> k-2, a(k-2) -> k-3, ..., a(k-6) -> k-7          */
            seq_t a1 = CACHE[k - 2];
            seq_t a2 = CACHE[k - 3];
            seq_t a3 = CACHE[k - 4];
            seq_t a4 = CACHE[k - 5];
            seq_t a5 = CACHE[k - 6];
            seq_t a6 = CACHE[k - 7];

            /* positive terms (8, 35, 12) */
            seq_t t1 = a1 * (seq_t)8;
            seq_t t3 = a3 * (seq_t)35;
            seq_t t5 = a5 * (seq_t)12;
            seq_t pos = t1 + t3 + t5;

            /* negative terms (24, 28, 2) */
            seq_t t2 = a2 * (seq_t)24;
            seq_t t4 = a4 * (seq_t)28;
            seq_t t6 = a6 * (seq_t)2;
            seq_t neg = t2 + t4 + t6;

            CACHE[k - 1] = pos - neg;                 /* a(k) */
        }
        CACHE_LEN = (size_t)n;
    }

    return CACHE[n - 1];
}
