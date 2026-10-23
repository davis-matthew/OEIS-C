/*
 * A024038  a(n) = 4^n - n^2.
 * https://oeis.org/A024038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - n^2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* compute 4^n using the provided utility */
    seq_t pow4 = seq_pow((seq_t)4, n);

    /* cast the index to seq_t for arithmetic */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;

    /* return the result */
    return pow4 - n2;
}
