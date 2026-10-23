/*
 * A047865  Number of derangements of n where minimal cycle size is at least
 *          4.
 * https://oeis.org/A047865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*a(n-1) + (n-1)*(n-2)*(n-3)*a(n-4) Initial values:
 *            a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=6
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cap = 0;        /* allocated size */
    static idx_t filled = 0;     /* highest index computed (inclusive) */

    /* initialize cache if first call */
    if (cache == NULL) {
        cap = 8;                       /* start with small size */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* fallback; not supposed to happen */

        cache[0] = (seq_t)1;
        cache[1] = cache[2] = cache[3] = (seq_t)0;
        cache[4] = (seq_t)6;
        filled = 4;
    }

    /* grow the array if needed */
    if (n >= cap) {
        size_t new_cap = (n + 1) * 2;   /* allocate a bit more to avoid frequent reallocs */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* fallback */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = filled + 1; i <= n; ++i) {
        if (i <= 3) {
            cache[i] = (seq_t)0;
        } else if (i == 4) {
            cache[i] = (seq_t)6;
        } else {
            seq_t term1 = (seq_t)(i - 1) * cache[i - 1];
            seq_t term2 = (seq_t)(i - 1) * (seq_t)(i - 2) *
                          (seq_t)(i - 3) * cache[i - 4];
            cache[i] = term1 + term2;
        }
    }

    if (n > filled)
        filled = n;

    return cache[n];
}
