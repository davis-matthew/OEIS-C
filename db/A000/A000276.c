/*
 * A000276  Associated Stirling numbers.
 * https://oeis.org/A000276
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = (n-1)!*Sum_{i=2..n-2} 1/i; computed by the recurrence
 *            f(m+1) = (m+1)*(f(m) + (m-1)!), where f(m) = a(m+1), f(3) = 3
 *            (a(4) = 3)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t f = 3;   /* f(3) = a(4) */
    seq_t g = 2;   /* (m-1)! with m = 3 */
    idx_t m;
    for (m = 3; m + 2 <= n; m++) {
        f = (f + g) * (seq_t)(m + 1);
        g *= (seq_t)m;
    }
    return f;
}
