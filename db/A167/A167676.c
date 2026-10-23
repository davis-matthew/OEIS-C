/*
 * A167676  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   mul_times
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the generating function
 * a(0) = 1
 * for 1 ≤ n ≤ 14:  a(n) = 2 + 17 * (a(n−1)+…+a(0))
 * a(15) = 1 + 17 * (a(14)+…+a(1)) – 153 * a(0)
 * for n ≥ 16:    a(n) = 17 * (a(n−1)+…+a(n−14)) – 153 * a(n−15)
 */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mul_times.h"

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;   /* number of terms stored (0..len-1) */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* Grow the cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 32)
            new_cap = 32;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort(); /* out‑of‑memory */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
            continue;
        }

        /* sum of previous terms (the sliding window) */
        seq_t sum_prev = 0;
        size_t kmax;
        if (i <= 14)          /* 1 … 14 */
            kmax = (size_t)i;
        else if (i == 15)     /* exactly 14 terms needed */
            kmax = 14;
        else                  /* i ≥ 16 */
            kmax = 14;

        for (size_t k = 1; k <= kmax; ++k) {
            /* avoid underflow when i < k (before cache filled) */
            if (i >= k)
                sum_prev += cache[(size_t)i - k];
        }

        seq_t term_result;
        if (i <= 14) {                    /* 1 … 14 */
            /* a(i) = 2 + 17 * sum_prev */
            term_result = 2 + mul_times(sum_prev, 17);
        } else if (i == 15) {             /* n = 15 */
            /* a(15) = 1 + 17 * sum_prev – 153 * a(0) */
            seq_t prod17  = mul_times(sum_prev, 17);
            seq_t prod153 = mul_times(cache[0], 153);
            term_result = 1 + prod17 - prod153;
        } else {                          /* i ≥ 16 */
            /* a(i) = 17 * sum_prev – 153 * a(i−15) */
            seq_t prod17  = mul_times(sum_prev, 17);
            seq_t prod153 = mul_times(cache[(size_t)i - 15], 153);
            term_result = prod17 - prod153;
        }

        cache[(size_t)i] = term_result;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
