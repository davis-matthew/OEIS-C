/*
 * A168674  a(n) = 2*A001610(n).
 * https://oeis.org/A168674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-3) (for n ≥ 3) * Initial values: a(0)=0,
 *            a(1)=4, a(2)=6
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Return the base cases directly. */
    if (n == 0) return 0;
    if (n == 1) return 4;
    if (n == 2) return 6;

    /* Use a rolling buffer of the last three terms to avoid
     * allocating a full array.  The values keep the exact type
     * seq_t that the build system provides.                     */
    seq_t prev3 = 0;  /* a(n-3) */
    seq_t prev2 = 4;  /* a(n-2) */
    seq_t prev1 = 6;  /* a(n-1) */
    seq_t cur;

    /* Iterate from 3 up to n, computing each successive term. */
    for (idx_t i = 3; i <= n; ++i) {
        cur   = 2 * prev1 - prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
