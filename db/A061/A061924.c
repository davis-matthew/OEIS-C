/*
 * A061924  Number of combinations in card games with 4 suits and 4 players.
 * https://oeis.org/A061924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n)! / (3*n)! (OEIS A061924)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n = 0 the product is empty → 1. */
    if (n == 0)
        return 1;

    seq_t result = 1;
    idx_t start = 3*n + 1;
    idx_t end   = 4*n;

    /* Compute product ∏_{k=3n+1}^{4n} k. */
    for (idx_t k = start; k <= end; ++k)
        result = result * (seq_t)k;

    return result;
}
