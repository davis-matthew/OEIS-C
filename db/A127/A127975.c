/*
 * A127975  Repeat 3^n three times.
 * https://oeis.org/A127975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^{floor(n/3)}
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 62, u64 n <= 122,
 *            u128 n <= 242, big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n) {
    /* floor division by 3 gives the exponent */
    return pow3(n / 3);
}
