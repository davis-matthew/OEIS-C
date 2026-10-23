/*
 * A041866  Numerators of continued fraction convergents to sqrt(455).
 * https://oeis.org/A041866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=21, a(1)=64, a(2)=2709, a(3)=8191, a(n)=128*a(n-2)-a(n-4) for n>=4 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the initial offset terms directly */
    if (n == 0) return (seq_t)21;
    if (n == 1) return (seq_t)64;
    if (n == 2) return (seq_t)2709;
    if (n == 3) return (seq_t)8191;

    /* Keep the last four computed values.
       a0 = a(k-4), a1 = a(k-3), a2 = a(k-2), a3 = a(k-1) */
    seq_t a0 = (seq_t)21;   /* a(0) */
    seq_t a1 = (seq_t)64;   /* a(1) */
    seq_t a2 = (seq_t)2709; /* a(2) */
    seq_t a3 = (seq_t)8191; /* a(3) */

    idx_t k;
    for (k = 4; k <= n; ++k)
    {
        /* a(k) = 128*a(k-2) - a(k-4) */
        seq_t new_val = (seq_t)128 * a2 - a0;

        /* Shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = new_val;
    }

    return a3;
}
