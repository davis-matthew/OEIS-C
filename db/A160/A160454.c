/*
 * A160454  Number of isomorphism classes of 5-fold coverings of a connected
 *          graph with Betti number n.
 * https://oeis.org/A160454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(n) = 4^(n-1) + 5^(n-1) + 2*6^(n-1) + 8^(n-1) +
 *            12^(n-1) + 120^(n-1) (for n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* exponent for the powers, cast to seq_t to stay in the value type */
    seq_t exp = (seq_t)(n - 1);

    /* compute the required powers */
    seq_t p4   = big_pow((seq_t)4,   exp);
    seq_t p5   = big_pow((seq_t)5,   exp);
    seq_t p6   = big_pow((seq_t)6,   exp);
    seq_t p8   = big_pow((seq_t)8,   exp);
    seq_t p12  = big_pow((seq_t)12,  exp);
    seq_t p120 = big_pow((seq_t)120, exp);

    /* sum according to the formula */
    seq_t result = p4;
    result += p5;
    result += seq_mul((seq_t)2, p6);
    result += p8;
    result += p12;
    result += p120;

    return result;
}
