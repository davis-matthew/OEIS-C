/*
 * A022163  First row of spectral array W(sqrt(3/2)).
 * https://oeis.org/A022163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-2) - 3*a(n-4),  a(0)=1, a(1)=5, a(2)=6, a(3)=27 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* handle the initial values explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)27;

    /* a sliding buffer of the last four computed values */
    seq_t buf[4];
    buf[0] = (seq_t)1;   /* a(0) */
    buf[1] = (seq_t)5;   /* a(1) */
    buf[2] = (seq_t)6;   /* a(2) */
    buf[3] = (seq_t)27;  /* a(3) */

    /* compute values up to n */
    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) depends on a(i-2) and a(i-4) */
        seq_t val = buf[(i - 2) % 4] * (seq_t)6
                  - buf[(i - 4) % 4] * (seq_t)3;
        buf[i % 4] = val;
    }

    return buf[n % 4];
}
