/*
 * A169526  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implements the OEIS formula for A169526.
 *  a(0) = 1
 *  a(n) = 33 * 32^(n-1)          for 1 ≤ n ≤ 33
 *  a(n) = 31 * (a(n-1)+…+a(n-33)) - 496 * a(n-34)   for n ≥ 34
 *  The recurrence follows from the denominator 496*t^34-31*t^33-…-31*t
 *  of the generating function G(t) = N(t)/D(t).
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of stored terms (max index + 1) */

    /* Allocate or enlarge cache if necessary */
    if (cache == NULL || n + 1 > cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cache = (seq_t*)malloc(new_len * sizeof(seq_t));
        if (new_cache == NULL)
            return (seq_t)0;   /* memory allocation failed */

        /* copy previously computed terms */
        if (cache != NULL) {
            memcpy(new_cache, cache, cache_len * sizeof(seq_t));
            free(cache);
        }
        cache      = new_cache;
        cache_len  = new_len;

        /* First term */
        if (cache_len > 0)
            cache[0] = (seq_t)1;

        /* Explicit formulas for the first 33 terms */
        seq_t p32 = (seq_t)1;  /* 32^(i-1) */
        for (size_t i = 1; i < cache_len && i <= 33; ++i) {
            cache[i] = (seq_t)33 * p32;     /* a(i) = 33 * 32^(i-1) */
            p32 = p32 * (seq_t)32;
        }

        /* If we have already reached the required index, finish */
        if (cache_len <= 34)
            return cache[n];

        /* Compute terms for n ≥ 34 using the linear recurrence.
         * Maintain a sliding sum of the last 33 terms to keep O(n) time. */
        seq_t sum33 = (seq_t)0;
        for (size_t k = 1; k <= 33; ++k)
            sum33 += cache[k];

        for (size_t i = 34; i < cache_len; ++i) {
            cache[i] = (seq_t)31 * sum33 - (seq_t)496 * cache[i - 34];
            /* Update sliding sum for the next index */
            sum33 += cache[i] - cache[i - 33];
        }
    }

    return cache[n];
}
