/*
 * A135492  Number of ways to toss a coin n times and not get a run of five.
 * https://oeis.org/A135492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-4) for n ≥ 5; initial values:
 *            a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=16
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 67, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* Handle the initial five values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)8;
    if (n == 4) return (seq_t)16;

    /* buffer holds a(k) at index k mod 5 for k <= n
       The array is initialized with the first five terms. */
    seq_t buf[5];
    buf[0] = (seq_t)1;   /* a(0) */
    buf[1] = (seq_t)2;   /* a(1) */
    buf[2] = (seq_t)4;   /* a(2) */
    buf[3] = (seq_t)8;   /* a(3) */
    buf[4] = (seq_t)16;  /* a(4) */

    /* Compute a(n) using the recurrence.  Each step writes the
       new term into buf[i % 5] and updates the current window. */
    for (idx_t i = 5; i <= n; ++i) {
        idx_t imod = i % 5;
        idx_t i1 = (i - 1) % 5;
        idx_t i2 = (i - 2) % 5;
        idx_t i3 = (i - 3) % 5;
        idx_t i4 = (i - 4) % 5;
        buf[imod] = buf[i1] + buf[i2] + buf[i3] + buf[i4];
    }

    return buf[n % 5];
}
