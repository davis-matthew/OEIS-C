/*
 * A137173  A006516 at positions with even indices, A007582 at positions with
 *          odd indices.
 * https://oeis.org/A137173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Define a(n) for n≥0 by a(2n)+a(2n+1)=4^n, a(2n+1)-a(2n)=2^n.
 *            Hence for n≥1 a(2n) = 2^{2n-1} – 2^{n-1}, a(2n+1) = 2^{2n-1} +
 *            2^{n-1}. This implements the definition of sequence A137173
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) = 0 */
    if (n == 1) return 1;          /* a(1) = 1 */

    idx_t m = n >> 1;              /* m = floor(n/2) */
    /* Compute powers of 2 needed: 2^(2m-1) and 2^(m-1) */
    seq_t pow1 = big_pow((seq_t)2, (seq_t)(2 * m - 1));
    seq_t pow2 = big_pow((seq_t)2, (seq_t)(m - 1));

    if ((n & 1) == 0)              /* even index: a(2m)  */
        return pow1 - pow2;
    else                            /* odd index:  a(2m+1) */
        return pow1 + pow2;
}
