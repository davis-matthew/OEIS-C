/*
 * A177109  a(n) = 4*(10^n-5).
 * https://oeis.org/A177109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(10^n-5)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n) {
    seq_t val = pow10(n);  /* 10^n */
    val -= 5;              /* 10^n - 5 */
    val *= 4;              /* 4*(10^n-5) */
    return val;
}
