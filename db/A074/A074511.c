/*
 * A074511  a(n) = 1^n + 4^n + 5^n.
 * https://oeis.org/A074511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1^n + 4^n + 5^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* 1^n is always 1 */
    seq_t one = 1;
    /* compute powers 4^n and 5^n using fast exponentiation */
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);
    seq_t pow5 = big_pow((seq_t)5, (seq_t)n);
    /* sum the terms */
    return one + pow4 + pow5;
}
