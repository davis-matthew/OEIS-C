/*
 * A199756  a(n) = 6*11^n + 1.
 * https://oeis.org/A199756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*11^n + 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    seq_t p = big_pow((seq_t)11, (seq_t)n);
    seq_t res = (seq_t)6 * p + (seq_t)1;
    return res;
}
