/*
 * A077244  Bisection (odd part) of Chebyshev sequence with Diophantine
 *          property.
 * https://oeis.org/A077244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - a(n-2); a(0) = 3, a(1) = 22
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)22;

    seq_t prev2 = (seq_t)3;   /* a(0) */
    seq_t prev1 = (seq_t)22;  /* a(1) */
    seq_t cur = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur   = (seq_t)8 * prev1 - prev2;  /* recurrence */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
