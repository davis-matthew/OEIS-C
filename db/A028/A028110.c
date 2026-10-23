/*
 * A028110  Expansion of 1/((1-4x)(1-5x)(1-6x)(1-8x)).
 * https://oeis.org/A028110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (8^(n+3)-6^(n+4)+8*5^(n+3)-3*4^(n+3))/24
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t exp1 = (seq_t)(n + 3);

    seq_t term1 = big_pow((seq_t)8, exp1);                 /* 8^(n+3)   */
    seq_t term2 = big_pow((seq_t)6, (seq_t)(n + 4));        /* 6^(n+4)   */
    seq_t term3 = (seq_t)8 * big_pow((seq_t)5, exp1);       /* 8*5^(n+3) */
    seq_t term4 = (seq_t)3 * big_pow((seq_t)4, exp1);       /* 3*4^(n+3) */

    seq_t numer = term1 - term2 + term3 - term4;
    return numer / (seq_t)24;
}
