/*
 * A094297  Number of (s(0), s(1), ..., s(n)) such that 0 < s(i) < 6 and |s(i)
 *          - s(i-1)| <= 1 for i = 1,2,...,n, s(0) = 2, s(n) = 2.
 * https://oeis.org/A094297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 4*a(n-3) with initial values
 *            a(1)=1, a(2)=3, a(3)=7
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)7;

    /* Sliding‑window recurrence. */
    seq_t a1 = (seq_t)1;   /* a(n-3) */
    seq_t a2 = (seq_t)3;   /* a(n-2) */
    seq_t a3 = (seq_t)7;   /* a(n-1) */
    seq_t a4;

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute a(n) = 4*a3 - 2*a2 - 4*a1 */
        a4 = (seq_t)4 * a3 - (seq_t)2 * a2 - (seq_t)4 * a1;
        /* Shift the window for the next iteration. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }

    return a3;
}
