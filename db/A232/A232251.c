/*
 * A232251  Number of (n+1) X (2+1) 0..2 arrays with every element both >= and
 *          <= some horizontal or antidiagonal neighbor.
 * https://oeis.org/A232251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) + 14*a(n-2) - 6*a(n-3) - a(n-4), with initial
 *            values a(1)=37, a(2)=421, a(3)=4711, a(4)=52779
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is first term. */
    if (n == 0)
        return 0;   /* Undefined for n<1; return 0 as a safe default. */

    if (n == 1) return (seq_t)37;
    if (n == 2) return (seq_t)421;
    if (n == 3) return (seq_t)4711;
    if (n == 4) return (seq_t)52779;

    /* Use four rolling variables for the recurrence. */
    seq_t v1 = (seq_t)37;    /* a(n-4)   */
    seq_t v2 = (seq_t)421;   /* a(n-3)   */
    seq_t v3 = (seq_t)4711;  /* a(n-2)   */
    seq_t v4 = (seq_t)52779; /* a(n-1)   */
    seq_t next;

    for (idx_t i = 5; i <= n; ++i) {
        /* Compute a(n) = 10*v4 + 14*v3 - 6*v2 - v1 */
        next = (seq_t)10 * v4 + (seq_t)14 * v3 - (seq_t)6 * v2 - v1;

        /* Shift the window for the next iteration. */
        v1 = v2;
        v2 = v3;
        v3 = v4;
        v4 = next;
    }

    return v4;
}
