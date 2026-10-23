/*
 * A185790  Number of (n+1) X 2 binary arrays with no 2 X 2 subblock sum equal
 *          to any horizontal or vertical neighbor 2 X 2 subblock sum.
 * https://oeis.org/A185790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-3) + 16*a(n-4) Initial values: a(1)=16,
 *            a(2)=40, a(3)=100, a(4)=260 The sequence satisfies this
 *            recurrence for all n ≥ 5
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 45, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For the small first terms we simply return the constant values. */
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)40;
    if (n == 3) return (seq_t)100;
    if (n == 4) return (seq_t)260;

    /* Compute the terms using the linear recurrence.  Only the five
       most recent values are needed, so we use a circular buffer of
       length 5. */
    seq_t ring[5] = {0, (seq_t)16, (seq_t)40, (seq_t)100, (seq_t)260};

    for (idx_t i = 5; i <= n; ++i) {
        ring[i % 5] =
            ring[(i - 1) % 5]                 /* a(i-1) */
          + ring[(i - 3) % 5] * (seq_t)4      /* 4*a(i-3) */
          + ring[(i - 4) % 5] * (seq_t)16;    /* 16*a(i-4) */
    }

    return ring[n % 5];
}
