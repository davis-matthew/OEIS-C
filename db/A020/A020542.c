/*
 * A020542  a(n) = 5th Chebyshev polynomial (second kind) evaluated at 2^n.
 * https://oeis.org/A020542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) * (3 - 2^(2n+4) + 2^(4n+4)) (5th Chebyshev
 *            polynomial of the second kind at 2^n)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    /* 2^(n+1) */
    seq_t p0 = pow2(n + 1);

    /* 2^(2n+4)  and  2^(4n+4) */
    seq_t p1 = pow2((idx_t)(2 * n + 4));
    seq_t p2 = pow2((idx_t)(4 * n + 4));

    /* 3 - 2^(2n+4) + 2^(4n+4) */
    seq_t inner = p2 - p1;
    inner += (seq_t)3;

    return p0 * inner;
}
