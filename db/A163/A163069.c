/*
 * A163069  a(n) = ((4+sqrt(5))*(1+sqrt(5))^n + (4-sqrt(5))*(1-sqrt(5))^n)/2.
 * https://oeis.org/A163069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2); a(0) = 4, a(1) = 9 (OEIS A163069)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the first two terms directly */
    if (n == 0)
        return (seq_t)4;
    if (n == 1)
        return (seq_t)9;

    /* iterate using the linear recurrence */
    seq_t prev2 = (seq_t)4; /* a(0) */
    seq_t prev1 = (seq_t)9; /* a(1) */
    seq_t cur   = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = (prev1 * (seq_t)2) + (prev2 * (seq_t)4);
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
