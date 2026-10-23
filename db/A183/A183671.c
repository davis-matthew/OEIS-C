/*
 * A183671  Number of (n+1) X 9 0..6 arrays with every 2 X 2 subblock summing
 *          to 12.
 * https://oeis.org/A183671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 64979929, a(n) = 7*7^n + 3060*6^n + 93300*5^n +
 *            893160*4^n + 4345560*3^n + 12867180*2^n + 22151340 for n ≥ 1
 * Range:     u8 none, u16 none, u32 n <= 5, u64 n <= 19, u128 n <= 44,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t result = (seq_t)0;
    seq_t term;

    /* 7 * 7^n */
    term = seq_pow((seq_t)7, n);
    result += term * (seq_t)7;

    /* 3060 * 6^n */
    term = seq_pow((seq_t)6, n);
    result += term * (seq_t)3060;

    /* 93300 * 5^n */
    term = seq_pow((seq_t)5, n);
    result += term * (seq_t)93300;

    /* 893160 * 4^n */
    term = seq_pow((seq_t)4, n);
    result += term * (seq_t)893160;

    /* 4345560 * 3^n */
    term = seq_pow((seq_t)3, n);
    result += term * (seq_t)4345560;

    /* 12867180 * 2^n */
    term = seq_pow((seq_t)2, n);
    result += term * (seq_t)12867180;

    /* constant term */
    result += (seq_t)22151340;

    return result;
}
