/*
 * A186562  Number of (n+2) X 5 0..2 arrays with every 3 X 3 subblock
 *          commuting with each horizontal and vertical neighbor 3 X 3
 *          subblock.
 * https://oeis.org/A186562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) - 4*a(n-4) + 2*a(n-5) +
 *            2*a(n-6) - 2*a(n-7) for n>11 Initial values: a(1)=2917,
 *            a(2)=401, a(3)=1399, a(4)=3421, a(5)=4807, a(6)=5605, a(7)=6511,
 *            a(8)=8029, a(9)=11647, a(10)=19237, a(11)=31623
 * Range:     u8 none, u16 n <= 12, u32 n <= 38, u64 n <= 90, u128 n <= 194,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Pre‑computed values for n = 1 … 11 */
    static const seq_t init[11] = {
        2917ULL, 401ULL, 1399ULL, 3421ULL, 4807ULL,
        5605ULL, 6511ULL, 8029ULL, 11647ULL, 19237ULL,
        31623ULL
    };

    /* Cached values for n ≥ 12 (one‑based indexing in cache) */
    static seq_t *cache = NULL;
    static idx_t cached = 0;   /* largest n already in cache */

    /* Return the pre‑computed value for small indices */
    if (n <= 11)
        return init[n - 1];

    /* Create or enlarge the cache as needed */
    if (!cache) {
        /* Allocate space for the requested n values and copy the 11 initial ones */
        cache = (seq_t *)malloc(sizeof(seq_t) * n);
        for (idx_t i = 0; i < 11; ++i)
            cache[i] = init[i];
        cached = 11;
    } else if (n > cached) {
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * n);
    }

    /* Compute missing terms using the empirical recurrence */
    for (idx_t k = cached + 1; k <= n; ++k) {
        /* a(k-1) … a(k-7) in cache are at indices k-2 … k-8 */
        seq_t a1 = cache[k - 2];   /* a(k-1) */
        seq_t a2 = cache[k - 3];   /* a(k-2) */
        seq_t a3 = cache[k - 4];   /* a(k-3) */
        seq_t a4 = cache[k - 5];   /* a(k-4) */
        seq_t a5 = cache[k - 6];   /* a(k-5) */
        seq_t a6 = cache[k - 7];   /* a(k-6) */
        seq_t a7 = cache[k - 8];   /* a(k-7) */

        /* Separate positive and negative contributions to avoid underflow */
        seq_t pos = 2 * a1 + 2 * a3 + 2 * a5 + 2 * a6;
        seq_t neg = a2 + 4 * a4 + 2 * a7;
        cache[k - 1] = pos - neg;
    }

    cached = n;
    return cache[n - 1];
}
