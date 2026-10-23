/*
 * A024069  a(n) = 6^n - n^7.
 * https://oeis.org/A024069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6^n - n^7
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 12, i64 n <= 24, i128 n <= 49,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"   /* big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    /* compute 6^n */
    seq_t pow6 = big_pow((seq_t)6, n);

    /* compute n^7 */
    seq_t nseq = (seq_t)n;
    seq_t n7 = nseq;
    for (idx_t i = 1; i < 7; ++i) {
        n7 *= nseq;
    }

    /* a(n) = 6^n - n^7 */
    return pow6 - n7;
}
