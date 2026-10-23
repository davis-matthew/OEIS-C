/*
 * A182241  a(n) = A161151(2*n)/2.
 * https://oeis.org/A182241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A161151(2*n)/2 Equivalent closed form: for n =
 *            (2*k-1)*2^p (p≥0) a(n) = 2^(2p+2)*k - 2*2^(2p) + 2^p (p is the
 *            exponent of the largest power of 2 dividing n)
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t t = n;
    idx_t p = 0;

    /* factor out the power of 2 */
    while ((t & 1ULL) == 0) {
        t >>= 1;
        ++p;
    }

    seq_t m = (seq_t)t;          /* odd part */
    seq_t k = (m + 1) >> 1;      /* (m+1)/2 */

    /* compute 2^p, 2^(2p), and 2^(2p+2) using only multiplication */
    seq_t pow2p = 1;
    for (idx_t i = 0; i < p; ++i) {
        pow2p *= 2;
    }
    seq_t pow2_2p  = pow2p * pow2p;   /* 2^(2p)   */
    seq_t pow2_2p2 = pow2_2p * 4;     /* 2^(2p+2) */

    seq_t result = pow2_2p2 * k;
    result -= (seq_t)2 * pow2_2p;
    result += pow2p;
    return result;
}
