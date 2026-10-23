/*
 * A100779  Expansion of
 *          (1+t^2+4*t^3+2*t^4+t^5+3*t^6)/((1-t)^2*(1-t^2)*(1-t^3)^2).
 * https://oeis.org/A100779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p_n − Σ_{i=1}^{10} d_i · a(n−i) where p_n is the
 *            coefficient of t^n in N(t) = 1 + t² + 4t³ + 2t⁴ + t⁵ + 3t⁶ (zero
 *            for n > 6), and d_i are the coefficients of D(t) =
 *            (1−t)²(1−t²)(1−t³)² = 1 − 2t + 0t² + 0t³ + 3t⁴ + 0t⁵ − 3t⁶ + 0t⁷
 *            + 0t⁸ + 2t⁹ − t¹⁰. The recurrence holds for all n ≥ 0 with a...
 * Range:     u8 n <= 8, u16 n <= 37, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Numerator coefficients p_n (degree ≤ 6) */
    static const seq_t num[7] = {
        (seq_t)1, 0, (seq_t)1, (seq_t)4,
        (seq_t)2, (seq_t)1, (seq_t)3
    };

    /* Denominator coefficients d_i, d[0] = 1 */
    static const int d[11] = {
        1, -2, 0, 0, 3, 0, -3, 0, 0, 2, -1
    };

    /* Cached sequence values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Extend cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;          /* allocation failed – unlikely */
        cache = tmp;
        /* initialize new entries to zero */
        memset(cache + cache_size,
               0,
               (new_size - cache_size) * sizeof(seq_t));

        /* compute terms from old cache_size to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val = 0;
            if (i < 7)
                val += num[i];

            /* apply recurrence using denominator coefficients */
            for (int j = 1; j <= 10; ++j) {
                if (i < (idx_t)j)
                    break;
                int di = d[j];
                if (di > 0) {
                    val -= (seq_t)di * cache[i - j];
                } else if (di < 0) {
                    val += (seq_t)(-di) * cache[i - j];
                }
            }
            cache[i] = val;
        }
        cache_size = new_size;
    }

    return cache[n];
}
