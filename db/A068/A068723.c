/*
 * A068723  Number of solenoidal flows (flow in = flow out) in a 4 X 4 square
 *          array with integer velocities -n .. n.
 * https://oeis.org/A068723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) -45*a(n-2) +120*a(n-3) -210*a(n-4) *
 *            +252*a(n-5) -210*a(n-6) +120*a(n-7) -45*a(n-8) +10*a(n-9) *
 *            -a(n-10) for n >= 10 * Initial values a(0) .. a(9) are taken
 *            from the OEIS entry
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 7, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* If cache size is smaller than needed, grow it */
    if (n + 1 > cache_len) {
        size_t new_len = n + 1;
        if (new_len < 10) new_len = 10;      /* keep a(0)..a(9) in cache */
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (cache == NULL) /* allocation failed */
            return (seq_t)0; /* fallback */

        /* Initial values for indices 0..9 */
        static const seq_t init[10] = {
            (seq_t)1,
            (seq_t)2053,
            (seq_t)160403,
            (seq_t)3096791,
            (seq_t)28905785,
            (seq_t)173429597,
            (seq_t)773568107,
            (seq_t)2789847759,
            (seq_t)8576204625,
            (seq_t)23279942261
        };

        /* Recurrence coefficients for a(n) when n >= 10 */
        static const int coeff[10] = {
            10,  -45,   120,  -210,   252,
           -210,  120,   -45,   10,   -1
        };

        /* Compute new entries */
        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i < 10) {
                cache[i] = init[i];
                continue;
            }
            /* Compute a(i) via recurrence */
            seq_t sum_pos = 0, sum_neg = 0;
            for (int k = 0; k < 10; ++k) {
                seq_t prod = cache[i - 1 - k] * (seq_t)((coeff[k] > 0) ? coeff[k] : -coeff[k]);
                if (coeff[k] > 0)
                    sum_pos += prod;
                else
                    sum_neg += prod;
            }
            cache[i] = sum_pos;
            cache[i] -= sum_neg;
        }
        cache_len = new_len;
    }
    return cache[n];
}
