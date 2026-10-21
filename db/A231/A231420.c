/*
 * A231420  Number of (1+1) X (n+1) 0..2 arrays with no element equal to a
 *          strict majority of its horizontal, diagonal and antidiagonal
 *          neighbors, with values 0..2 introduced in row major order.
 * https://oeis.org/A231420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A231420: a(1)=9, a(2)=71, a(3)=514, a(4)=3838, a(5)=28486, a(6)=212060
   Recurrence: a(n) = 8*a(n-1) + 4*a(n-2) - 58*a(n-3) - 24*a(n-4)
                 + 40*a(n-5) - 16*a(n-6)   for n >= 7   */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for n < 1; return 0 as a safeguard */
        return (seq_t)0;

    /* Base values a(1) .. a(6) */
    seq_t buf[6] = {(seq_t)9, (seq_t)71, (seq_t)514,
                    (seq_t)3838, (seq_t)28486, (seq_t)212060};

    if (n <= 6)
        return buf[n - 1];

    /* compute a(n) iteratively keeping the last six values */
    for (idx_t i = 7; i <= n; ++i) {
        seq_t next = 8 * buf[5] + 4 * buf[4] - 58 * buf[3]
                   - 24 * buf[2] + 40 * buf[1] - 16 * buf[0];

        /* shift buffer left: buf[0] = buf[1], … buf[4] = buf[5] */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = next;
    }

    return buf[5];  /* a(n) is stored in buf[5] after loop finishes */
}
