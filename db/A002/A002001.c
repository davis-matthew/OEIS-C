/*
 * A002001  a(n) = 3*4^(n-1), n>0; a(0)=1.
 * https://oeis.org/A002001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*4^(n-1) for n>0; a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    return (seq_t)3 * pow4(n - 1);
}
