/*
 * A004171  a(n) = 2^(2n+1).
 * https://oeis.org/A004171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(2n+1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   A001832_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A001832_pow2.h"

seq_t a(idx_t n) {
    return A001832_pow2(2*n + 1);
}
