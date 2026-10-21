/*
 * A051621  a(n) = (4*n+9)(!^4)/9(!^4), related to A007696(n+1) ((4*n+1)(!^4)
 *          quartic, or 4-factorials).
 * https://oeis.org/A051621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)*(4*n+5), with a(0) = 1 (this is equivalent to a(n)
 *            = Prod_{i=0}^{n+2} (4*i+1) / 45, satisfying a(n) =
 *            ((4*n+9)(!^4))/9(!^4).)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t val = 1;             /* a(0) */
    for (idx_t i = 0; i < n; ++i) {
        /* multiply by (4*i + 13) = 4*(i) + 13 */
        val *= (seq_t)(4 * i + 13);
    }
    return val;
}
