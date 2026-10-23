/*
 * A048508  a(n) = T(3,n), array T given by A048505.
 * https://oeis.org/A048508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+4)*(n+9) * 2^(n-2) - 8
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 24, u64 n <= 54, u128 n <= 116,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute (n+4)*(n+9) as a seq_t */
    seq_t mult = ((seq_t)n + 4) * ((seq_t)n + 9);

    /* 2^(n-2) is 1/4 or 1/2 for n = 0,1; otherwise integer */
    seq_t term;
    if (n < 2) {
        /* denominator is 2^(2-n) : 4 for n=0, 2 for n=1 */
        term = mult / pow2(2 - n);
    } else {
        term = mult * pow2(n - 2);
    }

    return term - (seq_t)8;
}
