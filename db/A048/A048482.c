/*
 * A048482  a(n) = T(n,n), array T given by A048472.
 * https://oeis.org/A048482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2+n)*2^(n-1)+1
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 53, u128 n <= 115,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;
    }
    seq_t nseq = (seq_t)n;
    seq_t t = nseq * (nseq + 1);
    seq_t pow = pow2(n - 1);
    return t * pow + 1;
}
