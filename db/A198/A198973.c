/*
 * A198973  a(n) = 8*10^n - 1.
 * https://oeis.org/A198973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*10^n - 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"

seq_t a(idx_t n) {
    seq_t ten_pow = pow10(n);
    return ten_pow * (seq_t)8 - (seq_t)1;
}
