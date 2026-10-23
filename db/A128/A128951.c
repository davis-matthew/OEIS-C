/*
 * A128951  a(n) is equal to the number of positive integers m less than or
 *          equal to 10^n such that m is not divisible by the prime 5 and is
 *          not divisible by at least one of the primes 2, 3 and 7.
 * https://oeis.org/A128951
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10^n - floor(10^n/5) - floor(10^n/42) + floor(10^n/210)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;    /* behavior for n < 2 is undefined, but safe to return 0 */
    seq_t p  = pow10(n);     /* 10^n */
    seq_t d5 = p / 5;        /* floor(10^n / 5) */
    seq_t d42 = p / 42;      /* floor(10^n / 42) */
    seq_t d210 = p / 210;    /* floor(10^n / 210) */
    return p - d5 - d42 + d210;
}
