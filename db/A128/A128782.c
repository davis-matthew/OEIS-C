/*
 * A128782  a(n) = n^2*4^n.
 * https://oeis.org/A128782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*4^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 58,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;          /* cast index to seq_t */
    seq_t pow = pow2(2 * n);        /* 4^n == 2^(2n) */
    return nseq * nseq * pow;       /* n^2 * 4^n */
}
