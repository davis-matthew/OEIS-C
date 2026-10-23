/*
 * A140961  Number of 3 X n {0,1}-matrices such that: (a) first and second row
 *          have a common 1, (b) second and third row have a common 1.
 * https://oeis.org/A140961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8^n - 2*6^n + 5^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* exponent as seq_t */
    seq_t exp = (seq_t)n;

    /* compute the powers */
    seq_t p8 = big_pow(8, exp);
    seq_t p6 = big_pow(6, exp);
    seq_t p5 = big_pow(5, exp);

    /* assemble the result */
    seq_t res = p8;
    res -= p6;   /* first subtraction of 6^n */
    res -= p6;   /* second subtraction of 6^n (total 2*6^n) */
    res += p5;   /* add 5^n */

    return res;
}
