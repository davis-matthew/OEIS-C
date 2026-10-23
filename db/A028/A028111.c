/*
 * A028111  Expansion of 1/((1-4x)(1-5x)(1-6x)(1-9x)).
 * https://oeis.org/A028111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (9^(n+3)-10*6^(n+3)+3*5^(n+4)-6*4^(n+3))/60
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;

    /* Compute each term of the numerator */
    seq_t term1 = big_pow((seq_t)9, nseq + (seq_t)3);                /* 9^(n+3)        */
    seq_t term2 = (seq_t)10 * big_pow((seq_t)6, nseq + (seq_t)3);    /* 10*6^(n+3)      */
    seq_t term3 = (seq_t)3 * big_pow((seq_t)5, nseq + (seq_t)4);     /* 3*5^(n+4)       */
    seq_t term4 = (seq_t)6 * big_pow((seq_t)4, nseq + (seq_t)3);     /* 6*4^(n+3)       */

    /* Combine to get the numerator, then divide by 60 */
    seq_t numerator = term1 - term2 + term3 - term4;
    seq_t result = numerator / (seq_t)60;

    return result;
}
