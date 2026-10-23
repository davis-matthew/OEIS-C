/*
 * A034488  Sum of n-th powers of divisors of 6.
 * https://oeis.org/A034488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1^n + 2^n + 3^n + 6^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t exp = (seq_t)n;         /* exponent */
    seq_t one = 1;                /* 1^n = 1 */
    seq_t two = 2;                /* 2^n */
    seq_t three = 3;              /* 3^n */
    seq_t six = 6;                /* 6^n */

    /* Compute powers using the provided big_pow helper */
    seq_t p2 = big_pow(two, exp);
    seq_t p3 = big_pow(three, exp);
    seq_t p6 = big_pow(six, exp);

    /* Sum all terms */
    return one + p2 + p3 + p6;
}
