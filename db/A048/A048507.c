/*
 * A048507  a(n) = T(2,n), array T given by A048505.
 * https://oeis.org/A048507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2+9*n+16)*2^(n-2)-3, a(0)=1, a(1)=10
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 24, u64 n <= 54, u128 n <= 116,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;
    seq_t nseq = (seq_t)n;
    seq_t base = nseq * nseq + (seq_t)9 * nseq + (seq_t)16;
    seq_t shift = pow2(n - 2);
    seq_t prod = base * shift;
    return prod - (seq_t)3;
}
