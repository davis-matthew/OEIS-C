/*
 * A022445  Number of self-avoiding closed walks (from 0 to 0) of length 2n in
 *          the strip {0, 1, 2} X Z of the square lattice Z X Z.
 * https://oeis.org/A022445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   Recurrence * a(0) = 1, a(1) = 0, a(2) = 4, a(3) = 10, a(4) = 34,
 *            * a(5) = 94, a(6) = 222, a(7) = 516. * For n >= 8: * a(n) =
 *            4*a(n-1) - 6*a(n-2) + 8*a(n-3) * - 9*a(n-4) + 4*a(n-5) -
 *            4*a(n-6)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow out-of-order calls */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;

    /* Grow cache to accommodate n */
    if (cache == NULL) {
        cache_cap = (n < 8) ? 8 : n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out-of-memory, unlikely in tests */
        cache_len = 0;
    } else if (n + 1 > cache_cap) {
        size_t new_cap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Precomputed numerator coefficients for the first 8 terms */
    static const int p[8] = {1, -4, 10, -14, 27, -18, 10, -6};

    /* Compute terms up to n if necessary */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t val = 0;
        if (i >= 1) val += cache[i-1] * (seq_t)4;
        if (i >= 2) {
            seq_t t = cache[i-2] * (seq_t)6;
            val -= t;
        }
        if (i >= 3) val += cache[i-3] * (seq_t)8;
        if (i >= 4) {
            seq_t t = cache[i-4] * (seq_t)9;
            val -= t;
        }
        if (i >= 5) val += cache[i-5] * (seq_t)4;
        if (i >= 6) {
            seq_t t = cache[i-6] * (seq_t)4;
            val -= t;
        }
        if (i <= 7) {
            int pi = p[i];
            if (pi >= 0)
                val += (seq_t)pi;
            else
                val -= (seq_t)(-pi);
        }
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
