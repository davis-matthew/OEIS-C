/*
 * A218986  Power floor sequence of 2+sqrt(7).
 * https://oeis.org/A218986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - a(n-2) - 3*a(n-3); Initial values: a(0) = 4,
 *            a(1) = 18, a(2) = 83
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)18;
    if (n == 2) return (seq_t)83;

    /* Use the recurrence a(n) = 5*a(n-1) - a(n-2) - 3*a(n-3) */
    seq_t a0 = (seq_t)4;   /* a(n-3) */
    seq_t a1 = (seq_t)18;  /* a(n-2) */
    seq_t a2 = (seq_t)83;  /* a(n-1) */
    seq_t a3;

    for (idx_t i = 3; i <= n; ++i) {
        a3 = 5 * a2 - a1 - 3 * a0;   /* compute a(n) */
        a0 = a1;   /* shift the window forward */
        a1 = a2;
        a2 = a3;
    }

    return a2;   /* a(n) after loop */
}
