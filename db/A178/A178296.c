/*
 * A178296  Number of collinear point 5-tuples in a 6 X 6 X 6 X...
 *          n-dimensional cubic grid.
 * https://oeis.org/A178296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10^n/2 + 2*8^n – 5*6^n/2 (offset 0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t exp = (seq_t)n;

    /* compute the required powers */
    seq_t pow10 = big_pow((seq_t)10, exp);
    seq_t pow8  = big_pow((seq_t)8,  exp);
    seq_t pow6  = big_pow((seq_t)6,  exp);

    /* apply the closed‑form formula */
    seq_t term = pow10 / (seq_t)2
               + ((seq_t)2 * pow8)
               - ((seq_t)5 * pow6) / (seq_t)2;

    return term;
}
