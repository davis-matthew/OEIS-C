/*
 * A054899  a(n) = Sum_{k>0} floor(n/10^k).
 * https://oeis.org/A054899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>0} floor(n/10^k) (OEIS A054899)
 * Range:     u8 n <= 2309, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the terminating nines function a(n) for n >= 0.
 * seq_t and idx_t are defined by the build environment.
 * seq_t may be 8/16/32/64-bit or arbitrary precision.
 */
seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t q = n;

    /* repeatedly divide by 10, accumulating the quotients
     * after the first division we obtain floor(n/10), then floor(n/100), etc.
     */
    while (q >= 10) {
        q /= 10;
        result += (seq_t)q;
    }

    return result;
}
