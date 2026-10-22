/*
 * A004119  a(0) = 1; thereafter a(n) = 3*2^(n-1) + 1.
 * https://oeis.org/A004119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = 3*2^(n-1) + 1 for n >= 1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   A001832_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A001832_pow2.h"


seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    return (seq_t)3 * A001832_pow2(n - 1) + (seq_t)1;
}
