/*
 * A186270  a(n)=Product{k=0..n, A003665(k)}.
 * https://oeis.org/A186270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=0..n} (4^k + (-2)^k)/2 (OEIS A186270)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)1;          /* a(0) = 1 */

    if (n == 0)
        return prod;

    seq_t pow2k = (seq_t)2;         /* 2^1 */

    for (idx_t k = 1; k <= n; ++k) /* multiply A003665(k) = 2^(k-1)*(2^k + (-1)^k) */
    {
        seq_t term;
        if ((k & 1) == 0)          /* k even : 2^(k-1)*(2^k + 1) */
            term = (pow2k >> 1) * (pow2k + (seq_t)1);
        else                       /* k odd  : 2^(k-1)*(2^k - 1) */
            term = (pow2k >> 1) * (pow2k - (seq_t)1);

        prod *= term;
        pow2k <<= 1;               /* next power of 2: 2^(k+1) */
    }

    return prod;
}
