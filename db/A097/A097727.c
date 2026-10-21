/*
 * A097727  Pell equation solutions (5*b(n))^2 - 26*a(n)^2 = -1 with b(n) =
 *          A097726(n), n >= 0.
 * https://oeis.org/A097727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 101, a(n) = 102*a(n-1) - a(n-2) for n >= 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)101;

    /* Iteratively compute the recurrence up to the requested index */
    seq_t prev2 = (seq_t)1;   /* a(n-2) */
    seq_t prev1 = (seq_t)101; /* a(n-1) */
    seq_t cur;

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        cur = prev1 * (seq_t)102 - prev2; /* a(n) = 102*a(n-1) - a(n-2) */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
