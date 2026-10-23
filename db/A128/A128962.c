/*
 * A128962  a(n) = (n^3 - n)*4^n.
 * https://oeis.org/A128962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 - n)*4^n, offset 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 25, u128 n <= 55,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* n >= 1 */
    seq_t nn = (seq_t)n;
    seq_t factor = nn * nn * nn - nn;      /* n^3 - n */
    seq_t pow4   = seq_pow((seq_t)4, n);   /* 4^n */
    return factor * pow4;
}
