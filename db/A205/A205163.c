/*
 * A205163  Number of (n+1) X 2 0..3 arrays with every 2 X 2 subblock having
 *          unequal diagonal elements or unequal antidiagonal elements, and
 *          new values 0..3 introduced in row major order.
 * https://oeis.org/A205163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=13, a(2)=159; for n≥3, a(n) = 18*a(n-1) - 45*a(n-2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the linear recurrence directly using
   integer arithmetic that works for any seq_t type provided
   by the build environment. */
#include <stdint.h>

/* External types supplied by the build system */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)13;
    if (n == 2)
        return (seq_t)159;

    seq_t prev2 = (seq_t)13;  /* a(n-2) */
    seq_t prev1 = (seq_t)159; /* a(n-1) */
    seq_t cur = 0;

    for (idx_t i = 3; i <= n; ++i) {
        /* a(i) = 18*a(i-1) - 45*a(i-2) */
        cur = prev1 * (seq_t)18 - prev2 * (seq_t)45;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
