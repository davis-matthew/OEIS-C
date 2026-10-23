/*
 * A100431  Bisection of A002417.
 * https://oeis.org/A100431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4/3)*(2*n^4 + 11*n^3 + 22*n^2 + 19*n + 6)
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 197, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a(n) = (8/3)*C(n+2,2)*C(2n+3,2) */

#include <stdint.h>
#include "../utils/binom2.h"

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t bin1 = binom2(nseq + 2);          /* C(n+2, 2) */
    seq_t two_n = nseq * 2;
    seq_t bin2 = binom2(two_n + 3);         /* C(2n+3, 2) */

    seq_t res = bin1 * bin2;
    res = res * 8;
    res = res / 3;
    return res;
}
