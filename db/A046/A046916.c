/*
 * A046916  a(n) = n*2^n + 2*n^2 + 1.
 * https://oeis.org/A046916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*2^n + 2*n^2 + 1
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;                      /* cast index to seq_t */
    seq_t term1 = sn * pow2(n);               /* n * 2^n               */
    seq_t term2 = ((seq_t)2 * sn * sn);       /* 2 * n^2                */
    return term1 + term2 + (seq_t)1;          /* + 1                    */
}
