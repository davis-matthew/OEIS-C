/*
 * A199317  a(n) = 2*6^n + 1.
 * https://oeis.org/A199317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*6^n + 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 6^n */
    seq_t pow6 = big_pow((seq_t)6, (seq_t)n);
    /* Multiply by 2 and add 1 */
    return pow6 * (seq_t)2 + (seq_t)1;
}
