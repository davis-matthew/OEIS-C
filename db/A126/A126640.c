/*
 * A126640  a(n) is the number of integers k less than 10^n such that the
 *          decimal representation of k lacks the digit 1 and 2, at least one
 *          of digits 3,4, at least one of digits 5,6 and at least one of
 *          digits 7,8,9.
 * https://oeis.org/A126640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 12*5^n-24*4^n+19*3^n-7*2^n+1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset starts at 1 – defensive */

    /* powers of the small bases */
    const seq_t five  = (seq_t)5;
    const seq_t four  = (seq_t)4;
    const seq_t three = (seq_t)3;
    const seq_t two   = (seq_t)2;

    seq_t p5 = big_pow(five,  (seq_t)n);
    seq_t p4 = big_pow(four,  (seq_t)n);
    seq_t p3 = big_pow(three, (seq_t)n);
    seq_t p2 = big_pow(two,   (seq_t)n);

    /* linear combination */
    seq_t val = (seq_t)12 * p5;
    val -= (seq_t)24 * p4;
    val += (seq_t)19 * p3;
    val -= (seq_t)7  * p2;
    val += (seq_t)1;

    return val;
}
