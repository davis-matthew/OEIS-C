/*
 * A060416  a(n) = n*4^n - 1, with a(0) = 1.
 * https://oeis.org/A060416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*4^n - 1, with a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t pow = pow4(n);
    seq_t res = (seq_t)n * pow - 1;
    return res;
}
