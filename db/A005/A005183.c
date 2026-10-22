/*
 * A005183  a(n) = n*2^(n-1) + 1.
 * https://oeis.org/A005183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*2^(n-1) + 1
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   A001832_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A001832_pow2.h"


seq_t a(idx_t n) {
    if (n == 0) {
        return 1;
    }
    return (seq_t)n * A001832_pow2(n - 1) + 1;
}
