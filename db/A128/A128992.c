/*
 * A128992  a(n) = (n^3 - n^2)*9^n.
 * https://oeis.org/A128992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 - n^2)*9^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 35,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* offset starts at 1 */
    seq_t nseq = (seq_t)n;
    seq_t coeff = nseq * nseq * (nseq - 1);     /* n^2 * (n-1) = n^3 - n^2 */
    seq_t pow9  = big_pow((seq_t)9, nseq);      /* 9^n */
    return coeff * pow9;
}
