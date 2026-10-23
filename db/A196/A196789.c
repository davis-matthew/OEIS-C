/*
 * A196789  Binomial coefficients C(2*n+10,10).
 * https://oeis.org/A196789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(2*n+10, 10)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 18, u64 n <= 188, u128 n <= 200,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    const idx_t k = 10;
    seq_t nprime = (seq_t)n * 2 + (seq_t)10;

    seq_t numerators[10];
    seq_t denominators[10];

    for (idx_t i = 0; i < k; ++i) {
        numerators[i]   = nprime - k + (i + 1); /* n' - k + 1 … n' */
        denominators[i] = (i + 1);               /* 1 … k       */
    }

    /* Cancel common factors between numerators and denominators */
    for (idx_t d = 0; d < k; ++d) {
        if (denominators[d] == (seq_t)1) continue;

        for (idx_t m = 0; m < k; ++m) {
            seq_t g = gcd(numerators[m], denominators[d]);
            if (g != (seq_t)1) {
                numerators[m]   /= g;
                denominators[d] /= g;
                if (denominators[d] == (seq_t)1) break;
            }
        }
    }

    /* All denominators should now be 1; multiply the remaining numerators */
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < k; ++i) {
        result *= numerators[i];
    }

    return result;
}
