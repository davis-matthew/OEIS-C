/*
 * A168251  a(n) = n^2 if n is odd, n^2*2^(n-2) if n is even.
 * https://oeis.org/A168251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 if n is odd; a(n) = n^2 * 2^(n-2) if n is even, with
 *            a(0)=0
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 55, u128 n <= 117,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the special case n == 0 */
    if (n == 0)
        return (seq_t)0;

    seq_t s = (seq_t)n;
    seq_t sq = s * s;

    /* Odd n → return n^2 */
    if (n & 1)
        return sq;

    /* Even n (>=2) → return n^2 * 2^(n-2) */
    seq_t p2 = pow2(n - 2);
    return sq * p2;
}
