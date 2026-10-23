/*
 * A013792  a(n) = 10^(4*n + 1).
 * https://oeis.org/A013792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^(4*n + 1)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n) {
    /* compute exponent 4*n + 1, safe because idx_t is unsigned 64-bit */
    idx_t exp = 4 * n + 1;
    return pow10(exp);
}
