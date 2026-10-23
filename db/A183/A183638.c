/*
 * A183638  Number of (n+1) X 6 0..3 arrays with every 2 X 2 subblock summing
 *          to 6.
 * https://oeis.org/A183638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2702 + 301*2^(n+2) + 62*3^(n+1) + 4^(n+1)
 * Range:     u8 none, u16 n <= 4, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* a(n) is defined only for n ≥ 1 */

    /* exponents as seq_t to match the signature of big_pow */
    seq_t exp_n1 = (seq_t)(n + 1);
    seq_t exp_n2 = (seq_t)(n + 2);

    /* compute the needed powers */
    seq_t pow2 = big_pow((seq_t)2, exp_n2);   /* 2^(n+2)  */
    seq_t pow3 = big_pow((seq_t)3, exp_n1);   /* 3^(n+1)  */
    seq_t pow4 = big_pow((seq_t)4, exp_n1);   /* 4^(n+1)  */

    /* assemble the result according to the closed form */
    seq_t result = (seq_t)2702;
    result += (seq_t)301 * pow2;
    result += (seq_t)62 * pow3;
    result += pow4;

    return result;
}
