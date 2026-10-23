/*
 * A161409  Number of reduced words of length n in the Weyl group E_6 on 6
 *          generators and order 51840.
 * https://oeis.org/A161409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in
 *            (1-x^2)(1-x^5)(1-x^6)(1-x^8)(1-x^9)(1-x^12)/(1-x)^6
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binom, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence counts reduced words of length n in the Weyl group E6. */
/* It is computed by convolution of the numerator polynomial with the binomial
   coefficients of 1/(1-x)^6, i.e. C(n-k+5,5). */

#include <stddef.h>
#include "../utils/binom.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* numerator coefficients of (1-x^2)(1-x^5)...(1-x^12) */
    static int64_t g[43];
    static int g_init = 0;

    if (!g_init) {
        /* initialize g[0] and progressively multiply by each factor (1-x^e) */
        const int exps[6] = {2,5,6,8,9,12};
        int max_idx = 0;
        g[0] = 1;
        for (int ei = 0; ei < 6; ++ei) {
            int e = exps[ei];
            /* For each existing coefficient, subtract it from the coefficient
               shifted by e positions.  Iterate backwards to avoid overwriting. */
            for (int j = max_idx; j >= 0; --j) {
                if (j + e <= 42) {
                    g[j + e] -= g[j];
                }
            }
            max_idx += e;
        }
        g_init = 1;
    }

    seq_t result = 0;
    idx_t limit = (n < 42 ? n : 42);
    for (idx_t k = 0; k <= limit; ++k) {
        seq_t bin = binom(n - k + 5, 5);
        if (g[k] >= 0) {
            result += mul_small(bin, (unsigned)g[k]);
        } else {
            result -= mul_small(bin, (unsigned)(-g[k]));
        }
    }
    return result;
}
