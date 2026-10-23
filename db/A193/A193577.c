/*
 * A193577  a(n) = 5*7^n.
 * https://oeis.org/A193577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*7^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   big_pow, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n) {
    /* Compute 7^n in the same type as the sequence. */
    seq_t pow7 = big_pow((seq_t)7, (seq_t)n);
    /* Multiply by the constant 5. */
    return mul_small(pow7, 5);
}
