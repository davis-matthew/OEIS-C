/*
 * A135264  a(n) = 3*A132357(n).
 * https://oeis.org/A135264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-3) + 3*a(n-4) Initial values: a(0)=3,
 *            a(1)=12, a(2)=42, a(3)=123
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Return the first few terms directly */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)12;
    if (n == 2) return (seq_t)42;
    if (n == 3) return (seq_t)123;

    /* Variables to hold the last four computed values:
       a[i-4], a[i-3], a[i-2], a[i-1] */
    seq_t a_minus4 = (seq_t)3;   /* a(0) */
    seq_t a_minus3 = (seq_t)12;  /* a(1) */
    seq_t a_minus2 = (seq_t)42;  /* a(2) */
    seq_t a_minus1 = (seq_t)123; /* a(3) */

    seq_t curr;

    /* Compute from i = 4 up to n */
    for (idx_t i = 4; i <= n; ++i) {
        curr = (3 * a_minus1) - a_minus3 + (3 * a_minus4);

        /* Shift the window for the next iteration */
        a_minus4 = a_minus3;
        a_minus3 = a_minus2;
        a_minus2 = a_minus1;
        a_minus1 = curr;
    }

    return a_minus1; /* a(n) is now held in a_minus1 */
}
