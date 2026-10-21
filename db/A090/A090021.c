/*
 * A090021  Number of distinct lines through the origin in the n-dimensional
 *          lattice of side length 5.
 * https://oeis.org/A090021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6^n - 3^n - 2*2^n + 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute the required powers.  All of these are positive and
       a sum of the form (6^n + 2) - (3^n + 2 * 2^n) is non‑negative
       for every n >= 0, so the sequence value can be built with
       unsigned arithmetic without underflow. */
    seq_t pow6 = big_pow((seq_t)6, (seq_t)n);
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);

    seq_t result = pow6;
    result += (seq_t)2;         /* 6^n + 2       */
    result -= pow3;             /* - 3^n         */
    result -= (pow2 << 1);      /* - 2*2^n       */

    return result;
}
