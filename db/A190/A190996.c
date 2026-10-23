/*
 * A190996  Fibonacci sequence beginning 10, 7.
 * https://oeis.org/A190996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 10, a(1) = 7
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the initial two terms */
    if (n == 0) return (seq_t)10;
    if (n == 1) return (seq_t)7;

    /* Iteratively compute the sequence using the recurrence */
    seq_t prev2 = (seq_t)10;   /* a(0) */
    seq_t prev1 = (seq_t)7;    /* a(1) */
    seq_t current;

    for (idx_t i = 2; i <= n; ++i) {
        current = prev2 + prev1; /* a(i) = a(i-2) + a(i-1) */
        prev2 = prev1;
        prev1 = current;
    }
    return prev1; /* a(n) */
}
