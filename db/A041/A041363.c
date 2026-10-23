/*
 * A041363  Denominators of continued fraction convergents to sqrt(195).
 * https://oeis.org/A041363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 28*a(n-2) - a(n-4) Initial values (offset 0): a(0) = 1
 *            a(1) = 1 a(2) = 27 a(3) = 28
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)27;
    if (n == 3) return (seq_t)28;

    /* Sliding window for the recurrence:
       a[0] = a(n-4), a[1] = a(n-3), a[2] = a(n-2), a[3] = a(n-1)  */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)27;  /* a(2) */
    seq_t a3 = (seq_t)28;  /* a(3) */
    seq_t ai = a3;         /* placeholder for a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        ai = (seq_t)28 * a2 - a0;   /* a(i) = 28*a(i-2) - a(i-4) */
        /* shift the window: a0 <- a1, a1 <- a2, a2 <- a3, a3 <- ai */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return ai;
}
