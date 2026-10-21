/*
 * A231213  Number of (n+1) X (2+1) 0..2 arrays with no element unequal to a
 *          strict majority of its horizontal and vertical neighbors, with
 *          values 0..2 introduced in row major order.
 * https://oeis.org/A231213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) with initial values
 *            a(1)=4, a(2)=9, a(3)=22, a(4)=59 offset: 1 (i.e. a(1) is the
 *            first listed term)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  /* needed for bool type in some builds */

seq_t a(idx_t n)
{
    /* Handle undefined index (offset starts at 1) */
    if (n < 1) return (seq_t)0;

    /* Direct return for the first four terms */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)22;
    if (n == 4) return (seq_t)59;

    /* Use a rolling window of the last four terms */
    seq_t A1 = (seq_t)4;  /* a(1) */
    seq_t A2 = (seq_t)9;  /* a(2) */
    seq_t A3 = (seq_t)22; /* a(3) */
    seq_t A4 = (seq_t)59; /* a(4) */

    seq_t An = 0;
    /* Compute a(5) .. a(n) iteratively */
    for (idx_t i = 5; i <= n; ++i) {
        An = (seq_t)3 * A4 - A3 + A2 - (seq_t)2 * A1;
        A1 = A2;
        A2 = A3;
        A3 = A4;
        A4 = An;
    }
    return An;
}
