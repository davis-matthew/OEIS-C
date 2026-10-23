/*
 * A115114  Asymmetric rhythm cycles (patterns): binary necklaces of length 2n
 *          subject to the restriction that for any k if the k-th bead is of
 *          color 1 then the (k+n)-th bead (modulo 2n) is of color 0.
 * https://oeis.org/A115114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (∑_{d|n} φ(2d) + ∑_{d|n, d odd} φ(d)·3^{n/d}) / (2n)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   phi, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility functions from the build */
#include <stdint.h>
#include "../utils/phi.h"   /* phi(idx_t) -> seq_t */
#include "../utils/pow3.h" /* pow3(idx_t) -> seq_t */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;

            /* first divisor d1 */
            sum += phi(2 * d1);
            if (d1 & 1U) {                       /* d1 is odd */
                sum += phi(d1) * pow3(n / d1);
            }

            /* second divisor d2, if distinct */
            if (d1 != d2) {
                sum += phi(2 * d2);
                if (d2 & 1U) {                   /* d2 is odd */
                    sum += phi(d2) * pow3(n / d2);
                }
            }
        }
    }
    seq_t denom = (seq_t)(2 * n);
    return sum / denom;
}
