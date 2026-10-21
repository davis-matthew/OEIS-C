/*
 * A239739  a(n) = n*4^(2*n+1).
 * https://oeis.org/A239739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*4^(2*n+1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    /* Compute 16^n */
    seq_t pow16 = big_pow((seq_t)16, (seq_t)n);
    /* Multiply by 4n */
    seq_t mult = (seq_t)4 * (seq_t)n;
    return pow16 * mult;
}
