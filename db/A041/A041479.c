/*
 * A041479  Denominators of continued fraction convergents to sqrt(255).
 * https://oeis.org/A041479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 32*a(n-2) - a(n-4), initial values: a(0)=1, a(1)=1,
 *            a(2)=31, a(3)=32
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the small, explicitly known values. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)31;
    if (n == 3) return (seq_t)32;

    /* Use a 4‑slot circular buffer to keep the last four terms. */
    seq_t buf[4];
    buf[0] = (seq_t)1;  /* a(0)      */
    buf[1] = (seq_t)1;  /* a(1)      */
    buf[2] = (seq_t)31; /* a(2)      */
    buf[3] = (seq_t)32; /* a(3)      */

    /* Compute terms from 4 up to n using the recurrence. */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = 32 * buf[(i - 2) & 3] - buf[(i - 4) & 3];
        buf[i & 3] = next;
    }

    /* The desired term is now in the buffer slot indexed by n mod 4. */
    return buf[n & 3];
}
