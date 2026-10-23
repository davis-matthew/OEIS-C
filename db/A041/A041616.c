/*
 * A041616  Numerators of continued fraction convergents to sqrt(327).
 * https://oeis.org/A041616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 434*a(n‑2) – a(n‑4) Initial values: a(0)=18, a(1)=217,
 *            a(2)=7830, a(3)=94177 (Recurrence requires n ≥ 4)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 0) return (seq_t)18;
    if (n == 1) return (seq_t)217;
    if (n == 2) return (seq_t)7830;
    if (n == 3) return (seq_t)94177;

    /* Circular buffer of the last four computed values:
       buf[0] = a(i-4), buf[1] = a(i-3), buf[2] = a(i-2), buf[3] = a(i-1) */
    seq_t buf[4];
    buf[0] = (seq_t)18;
    buf[1] = (seq_t)217;
    buf[2] = (seq_t)7830;
    buf[3] = (seq_t)94177;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t cur = (seq_t)434 * buf[2] - buf[0];  /* a(i) = 434*a(i-2) – a(i-4) */

        /* Shift the buffer for the next iteration */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = cur;
    }

    return buf[3];  /* after the loop buf[3] holds a(n) */
}
