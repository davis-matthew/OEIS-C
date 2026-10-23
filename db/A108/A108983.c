/*
 * A108983  Inverse binomial transform of A003950.
 * https://oeis.org/A108983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*6^n - (-1)^n)/7 Equivalent recurrence: a(0) = 1 a(1) =
 *            7 a(n) = 5*a(n-1) + 6*a(n-2) for n >= 2 This implementation uses
 *            the recurrence to avoid temporary values that would overflow
 *            smaller integer types
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;

    seq_t prev2 = (seq_t)1;   // a(n-2)
    seq_t prev1 = (seq_t)7;   // a(n-1)
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = 5 * prev1 + 6 * prev2;   // a(n) = 5*a(n-1) + 6*a(n-2)
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
