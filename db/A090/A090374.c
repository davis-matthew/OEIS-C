/*
 * A090374  Number of rooted planar 4-constellations with n quadrangles:
 *          rooted planar maps with bicolored faces having n black
 *          quadrangular faces and an arbitrary number of white faces of
 *          degrees multiple to 4.
 * https://oeis.org/A090374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*4^(n-1)*binomial(4*n, n)/((3*n+1)*(3*n+2))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(n)=32*(4*n-3)*(2*n-1)*(4*n-1)/(3*n*(3*n+1)*(3*n+2))*a(n-1) */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* values for n=0 are irrelevant */
    if (n == 1) return 1;          /* a(1) = 1 */

    seq_t val = 1;                 /* holds a(n-1) during the loop */

    for (idx_t i = 2; i <= n; ++i) {
        /* numerical and denominator factors for the recurrence */
        seq_t num[4] = {
            (seq_t)32,
            (seq_t)(4*i - 3),
            (seq_t)(2*i - 1),
            (seq_t)(4*i - 1)
        };
        seq_t den[3] = {
            (seq_t)(3*i),
            (seq_t)(3*i + 1),
            (seq_t)(3*i + 2)
        };

        /* Reduce the denominators using the current value (a(n-1)) */
        for (int d = 0; d < 3; ++d) {
            seq_t g = gcd(val, den[d]);
            if (g != 1) {
                val /= g;
                den[d] /= g;
            }
        }

        /* Reduce each numerator against the remaining denominators */
        for (int k = 0; k < 4; ++k) {
            seq_t curr = num[k];
            for (int d = 0; d < 3; ++d) {
                seq_t g = gcd(curr, den[d]);
                if (g != 1) {
                    curr /= g;
                    den[d] /= g;
                }
            }
            /* Multiply the reduced numerator into the running value */
            val *= curr;
        }

        /* Any remaining denominator factors should now be 1;
           divide them just in case due to cancellation order. */
        for (int d = 0; d < 3; ++d) {
            if (den[d] != 1) {
                val /= den[d];
            }
        }
    }

    return val;
}
