/*
 * A213563  Antidiagonal sums of the convolution array A213561.
 * https://oeis.org/A213563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binom(n+5,6) + 3*binom(n+4,6) + 2*binom(n+3,6)
 *            Recurrence: a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4)
 *            + 21*a(n-5) - 7*a(n-6) + a(n-7) Initial values: a(1)=1, a(2)=10,
 *            a(3)=51, a(4)=182, a(5)=518, a(6)=1260, a(7)=2730
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 88, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom_6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_6.h"

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is not defined. Return 0 for safety. */
    if (n < 1) return 0;

    seq_t t1 = binom_6(n + 5);      /* binom(n+5,6)   */
    seq_t t2 = ((seq_t)3) * binom_6(n + 4); /* 3 * binom(n+4,6) */
    seq_t t3 = ((seq_t)2) * binom_6(n + 3); /* 2 * binom(n+3,6) */

    return t1 + t2 + t3;
}
