/*
 * A020869  Number of single component forests in Moebius ladder M_n.
 * https://oeis.org/A020869
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence for A020869 (single component forests in the Möbius ladder M_n):
 * Let d[0..9] = {1, -13, 64, -156, 218, -190, 108, -40, 9, -1}
 * Let c[0..10] = {0, 0, 34, -220, 570, -781, 663, -360, 126, -27, 3}
 *
 * a_0 = a_1 = 0
 * For 2 ≤ n ≤ 10:
 *      a_n = c[n] - Σ_{k=1}^{min(9,n-2)} d[k] * a_{n-k}
 * For n ≥ 11:
 *      a_n = - Σ_{k=1}^{9} d[k] * a_{n-k}
 *
 * The first term defined at offset 2, so a(2)=34, a(3)=222, …
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* cache for already computed values */
    static seq_t *cache = NULL;
    static size_t cache_size  = 0;   /* highest index +1 that is valid */
    static size_t cache_cap   = 0;   /* capacity of the cache array */

    if (n < 2) {
        /* values below the offset are not defined; return 0 */
        return (seq_t)0;
    }

    /* ensure cache has room up to n */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < n + 1)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* if cache was empty, initialise a0 and a1 */
    if (cache_size == 0) {
        if (cache_cap < 2) {
            cache = (seq_t *)realloc(cache, 2 * sizeof(seq_t));
            cache_cap = 2;
        }
        cache[0] = cache[1] = (seq_t)0;
        cache_size = 2;
    }

    /* pre‑compute constants */
    static const int d[10] = { 1, -13, 64, -156, 218, -190, 108, -40, 9, -1 };
    static const int c[11] = { 0, 0, 34, -220, 570, -781, 663, -360, 126, -27, 3 };

    /* compute terms up to n */
    idx_t i;
    for (i = cache_size; i <= n; ++i) {
        seq_t sum = (seq_t)0;          /* running total */

        if (i <= 10) {
            /* contribution of the inhomogeneous part c[i] */
            int ci = c[i];
            if (ci > 0)
                sum += (seq_t)ci;
            else
                sum -= (seq_t)(-ci);

            /* contributions from earlier terms */
            idx_t maxk = (i > 2) ? ((i - 2) < 9 ? (i - 2) : 9) : 0;
            for (int k = 1; k <= maxk; ++k) {
                int dk = d[k];
                seq_t mult = cache[i - k] * (seq_t)(dk < 0 ? -dk : dk);
                if (dk < 0)
                    sum += mult;     /* -d_k is positive */
                else
                    sum -= mult;     /* subtract positive d_k */
            }
        } else {
            /* homogeneous recurrence for i >= 11 */
            for (int k = 1; k <= 9; ++k) {
                int dk = d[k];
                seq_t mult = cache[i - k] * (seq_t)(dk < 0 ? -dk : dk);
                if (dk < 0)
                    sum += mult;
                else
                    sum -= mult;
            }
        }
        cache[i] = sum;
    }

    cache_size = (size_t)n + 1;
    return cache[n];
}
