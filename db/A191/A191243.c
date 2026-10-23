/*
 * A191243  Reversion of x-x^2-x^3-x^4-x^5-x^6.
 * https://oeis.org/A191243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 For n ≥ 2, a(n) is the coefficient of xⁿ in the
 *            reversion of x – x² – x³ – x⁴ – x⁵ – x⁶, computed by the
 *            recurrence a(n) = b₂(n) + b₃(n) + b₄(n) + b₅(n) + b₆(n) where
 *            bₖ(n) = Σ_{i=1}^{n-1} a(i)·b_{k-1}(n-i) (b₂ is the convolution
 *            of a with itself). This recurrence follows from the defini...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 29, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Caches of computed values. */
    static seq_t *a_cache = NULL, *b2_cache = NULL,
                *b3_cache = NULL, *b4_cache = NULL,
                *b5_cache = NULL, *b6_cache = NULL;
    static idx_t cache_len = 0;    /* highest index computed so far */
    static size_t cache_cap = 0;   /* allocated capacity of each array */

    /* Grow caches if needed. */
    if (cache_len < n) {
        size_t needed = (size_t)n + 1;   /* indices 0..n */
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < needed) new_cap <<= 1;
            a_cache   = (seq_t*)realloc(a_cache,   new_cap * sizeof(seq_t));
            b2_cache  = (seq_t*)realloc(b2_cache,  new_cap * sizeof(seq_t));
            b3_cache  = (seq_t*)realloc(b3_cache,  new_cap * sizeof(seq_t));
            b4_cache  = (seq_t*)realloc(b4_cache,  new_cap * sizeof(seq_t));
            b5_cache  = (seq_t*)realloc(b5_cache,  new_cap * sizeof(seq_t));
            b6_cache  = (seq_t*)realloc(b6_cache,  new_cap * sizeof(seq_t));
            cache_cap = new_cap;
            /* Initialize newly allocated slots to zero. */
            for (size_t i = cache_len + 1; i < needed; ++i) {
                a_cache[i]  = 0;
                b2_cache[i] = 0;
                b3_cache[i] = 0;
                b4_cache[i] = 0;
                b5_cache[i] = 0;
                b6_cache[i] = 0;
            }
        }

        for (idx_t m = cache_len + 1; m <= n; ++m) {
            if (m == 1) {
                a_cache[1] = 1;          /* initial term */
                /* b2..b6 remain 0 for index 1 */
                continue;
            }

            seq_t sum2 = 0, sum3 = 0, sum4 = 0, sum5 = 0, sum6 = 0;
            for (idx_t k = 1; k < m; ++k) {
                seq_t ak   = a_cache[k];
                seq_t amk  = a_cache[m - k];
                sum2 += ak * amk;                /* b2 */
                sum3 += ak * b2_cache[m - k];    /* b3 */
                sum4 += ak * b3_cache[m - k];    /* b4 */
                sum5 += ak * b4_cache[m - k];    /* b5 */
                sum6 += ak * b5_cache[m - k];    /* b6 */
            }
            b2_cache[m] = sum2;
            b3_cache[m] = sum3;
            b4_cache[m] = sum4;
            b5_cache[m] = sum5;
            b6_cache[m] = sum6;
            a_cache[m]  = sum2 + sum3 + sum4 + sum5 + sum6;
        }

        cache_len = n;
    }

    return a_cache[n];
}
