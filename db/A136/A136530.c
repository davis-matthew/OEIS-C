/*
 * A136530  a(n) = 2^n*(3*n^2 + 13*n + 8)/8.
 * https://oeis.org/A136530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 53, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A136530
 * Formula: a(n) = 2^n * (3*n^2 + 13*n + 8) / 8
 * Recurrence: a(0)=1, a(1)=6, a(2)=23,
 *             a(n)=6*a(n-1)-12*a(n-2)+8*a(n-3) for n≥3
 */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)23;

    seq_t a0 = (seq_t)1,       /* a(n-3)   */
           a1 = (seq_t)6,       /* a(n-2)   */
           a2 = (seq_t)23,      /* a(n-1)   */
           next;

    for (idx_t i = 3; i <= n; ++i) {
        /* a(i) = 6*a(i-1) - 12*a(i-2) + 8*a(i-3) */
        next = (seq_t)6 * a2 - (seq_t)12 * a1 + (seq_t)8 * a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
