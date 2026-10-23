/*
 * A049332  Number of conjugacy classes in Clifford group CL(n).
 * https://oeis.org/A049332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 2^n + (n even ? 1 : 2)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    seq_t result = pow2(n);           /* result = 2^n   */
    result += (n & 1) ? 2 : 1;        /* add 2 if odd, else 1 */
    return result;
}
