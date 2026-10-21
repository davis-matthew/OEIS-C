/*
 * A071226  n-th power of the product of next n natural numbers.
 * https://oeis.org/A071226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [∏_{k=1}^{n} ( n(n-1)/2 + k ) ]^n i.e. take the n natural
 *            numbers immediately following the (n-1)-st triangular number,
 *            multiply them together, and raise the product to the n-th power
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* not needed for the offset but harmless */
    seq_t prod = 1;

    /* compute the starting integer: start = T_{n-1} + 1 = n(n-1)/2 + 1 */
    idx_t start = ( ( (unsigned __int128)n * (n-1) ) / 2 ) + 1;

    for (idx_t i = 0; i < n; ++i)
    {
        prod *= (seq_t)(start + i);
    }

    return seq_pow(prod, n);
}
