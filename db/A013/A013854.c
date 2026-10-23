/*
 * A013854  a(n) = 10^(5*n + 1).
 * https://oeis.org/A013854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^(5*n + 1)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n) {
    /* exponent = 5*n + 1 */
    idx_t exp = 5 * n + 1;
    return pow10(exp);
}
