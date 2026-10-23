/*
 * A034672  Sum of n-th powers of divisors of 96.
 * https://oeis.org/A034672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{d|96} d^n, i.e. the sum of the n-th powers of the
 *            divisors of 96. For n=0 this is simply the divisor count 12
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"   /* provides seq_t big_pow(seq_t, seq_t); */

seq_t a(idx_t n)
{
    if (n == 0)          /* every divisor to the 0-th power is 1 */
        return (seq_t)12;

    /* Compute 2^n and 3^n using arbitrary‑precision power */
    seq_t two_n  = big_pow((seq_t)2, (seq_t)n);
    seq_t three_n = big_pow((seq_t)3, (seq_t)n);

    /* Sum over the exponent of 2: a = 0..5  (i.e. 2^0 * 3^b)   */
    seq_t sum2 = (seq_t)1;
    seq_t term = (seq_t)1;        /* term holds (2^n)^a  */
    for (idx_t a = 1; a <= 5; ++a) {
        term = term * two_n;
        sum2 += term;
    }

    /* Sum over the exponent of 3: b = 0..1  (i.e. 3^0 * 2^a)   */
    seq_t sum3 = (seq_t)1;
    term = (seq_t)1;              /* term holds (3^n)^b  */
    for (idx_t b = 1; b <= 1; ++b) {
        term = term * three_n;
        sum3 += term;
    }

    return sum2 * sum3;   /* product of the two geometric sums */
}
