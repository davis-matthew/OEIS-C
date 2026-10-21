/*
 * A237419  a(n) = 4^(n+1)^2 - 3.
 * https://oeis.org/A237419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^((n+1)^2) - 3
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    seq_t exp = (seq_t)(n + 1) * (seq_t)(n + 1);
    seq_t result = big_pow((seq_t)4, exp);
    result -= (seq_t)3;
    return result;
}
