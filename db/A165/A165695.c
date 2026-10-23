/*
 * A165695  Number of reduced words of length n in Coxeter group on 44
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=44, a(2)=1892, a(3)=81356, a(4)=3498308,
 *            a(5)=150427244, a(6)=6468371492, a(7)=278139974156,
 *            a(8)=11960018888708; a(9)=42*(a(1)+⋯+a(8)) – 903*a(0) + 1;
 *            a(n)=42*(a(n‑1)+⋯+a(n‑8)) – 903*a(n‑9) for n ≥ 10
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;   /* number of entries stored in cache */

    /* Initialise cache with the first 10 terms (0‑based). */
    if (cache == NULL) {
        cache_size = 10;                     /* indices 0 … 9 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed */

        cache[0] = (seq_t)1;                  /* a(0) */
        seq_t val = (seq_t)44;                /* a(1) */
        cache[1] = val;
        for (idx_t i = 2; i <= 8; ++i) {      /* a(2)…a(8) = 44*43^(i-1) */
            val *= (seq_t)43;
            cache[i] = val;
        }

        /* compute a(9) using the inhomogeneous part of the recurrence */
        seq_t sum = (seq_t)0;
        for (int k = 1; k <= 8; ++k)
            sum += cache[k];
        cache[9] = (seq_t)42 * sum - (seq_t)903 * cache[0] + (seq_t)1;
    }

    /* If the requested index is already cached, return it immediately. */
    if ((size_t)n < cache_size)
        return cache[n];

    /* Grow the cache to hold indices up to n. */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;          /* allocation failed */
    cache = tmp;

    /* Compute missing terms from the highest cached index up to n. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t s = (seq_t)0;
        for (int k = 1; k <= 8; ++k)
            s += cache[(size_t)(i - k)];
        cache[(size_t)i] = (seq_t)42 * s - (seq_t)903 * cache[(size_t)(i - 9)];
    }

    cache_size = new_size;
    return cache[(size_t)n];
}
