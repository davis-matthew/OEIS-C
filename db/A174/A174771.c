/*
 * A174771  y-values in the solution to x^2 - 31*y^2 = 1.
 * https://oeis.org/A174771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3040*a(n-1) - a(n-2); a(1)=0, a(2)=273
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 2)
    {
        return (n == 1) ? (seq_t)0 : (seq_t)273;
    }

    seq_t prev2 = (seq_t)0;   /* a(1) */
    seq_t prev  = (seq_t)273; /* a(2) */
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i)
    {
        curr = ((seq_t)3040 * prev) - prev2;
        prev2 = prev;
        prev  = curr;
    }

    return prev;
}
