/*
 * A034496  Sum of n-th powers of divisors of 8.
 * https://oeis.org/A034496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 2^n + 4^n + 8^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute powers of 2: 2^n, 4^n = 2^(2n), 8^n = 2^(3n) */
    seq_t term = 1;               /* the constant 1 */
    term += pow2(n);              /* 2^n */
    term += pow2(2 * n);          /* 4^n */
    term += pow2(3 * n);          /* 8^n */
    return term;
}
