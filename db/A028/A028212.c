/*
 * A028212  Expansion of 1/((1-6x)(1-8x)(1-9x)(1-12x)).
 * https://oeis.org/A028212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^(n+2) - 9^(n+2) + 2*12^(n+1) - 6^(n+1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Compute each power term */
    seq_t pow8   = seq_pow((seq_t)8, n + 2);
    seq_t pow9   = seq_pow((seq_t)9, n + 2);
    seq_t pow12  = seq_pow((seq_t)12, n + 1);
    seq_t pow6   = seq_pow((seq_t)6, n + 1);

    /* Combine them according to the formula */
    seq_t result = pow8 - pow9 + (seq_t)2 * pow12 - pow6;

    return result;
}
