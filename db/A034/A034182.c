/*
 * A034182  Number of not-necessarily-symmetric n X 2 crossword puzzle grids.
 * https://oeis.org/A034182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 4 (a(1)=1, a(2)=5)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 0)
        return (seq_t)0;          /* undefined for n < 1, but safe */

    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)5;

    seq_t prev2 = (seq_t)1;       /* a(1) */
    seq_t prev1 = (seq_t)5;       /* a(2) */
    seq_t curr  = (seq_t)0;

    for (idx_t i = 3; i <= n; ++i)
    {
        curr = (seq_t)2 * prev1 + prev2 + (seq_t)4;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
