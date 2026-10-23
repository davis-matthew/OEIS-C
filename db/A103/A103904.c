/*
 * A103904  a(n) = n*(n-1)/2 * 2^(n*(n-1)/2).
 * https://oeis.org/A103904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-1)/2 * 2^(n*(n-1)/2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute n choose 2 as seq_t */
    seq_t tri = (seq_t)n * (seq_t)(n-1) / 2;

    /* 2^tri as seq_t */
    seq_t pw = pow2((idx_t)tri);

    /* a(n) = tri * 2^tri */
    return pw * tri;
}
