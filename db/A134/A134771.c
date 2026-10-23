/*
 * A134771  A134770 interleaved with threes.
 * https://oeis.org/A134771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 2*(1 + (-1)^n)*binomial(n, n/2) - 3*(-1)^n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n & 1U) {              /* odd n => a(n) = 3 */
        return (seq_t)3;
    } else {                   /* even n => a(n) = 4*C(n, n/2) - 3 */
        seq_t c = binomial(n, n/2);
        return c * (seq_t)4 - (seq_t)3;
    }
}
