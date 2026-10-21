/*
 * A238471  a(n) = binomial(5n+6, 4)/5 for n >= 0.
 * https://oeis.org/A238471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(5*n+6,4)/5 = (5*n+6)*(5*n+5)*(5*n+4)*(5*n+3)/120
 * Range:     u8 none, u16 n <= 2, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute the product (5n+6)(5n+5)(5n+4)(5n+3) */
    seq_t five_n = (seq_t)n * 5;
    seq_t t1 = five_n + 6;
    seq_t t2 = five_n + 5;
    seq_t t3 = five_n + 4;
    seq_t t4 = five_n + 3;

    seq_t prod = t1 * t2;
    prod = prod * t3;
    prod = prod * t4;

    /* divide by 120 (= 5 * 24) – the division is exact by construction */
    return prod / 120;
}
