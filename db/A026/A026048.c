/*
 * A026048  (d(n)-r(n))/5, where d = A026046 and r is the periodic sequence
 *          with fundamental period (1,0,4,0,0).
 * https://oeis.org/A026048
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (d(n) - r(n)) / 5 Recurrence derived from the generating
 *            function: a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + a(n-5) -
 *            3*a(n-6) + 3*a(n-7) - a(n-8) Initial values (n = 6 to 13):
 *            a(6)=15, a(7)=21, a(8)=28, a(9)=39, a(10)=52, a(11)=68,
 *            a(12)=88, a(13)=111 The function returns the value for any n...
 * Range:     u8 n <= 17, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle indices below the offset; behaviour is irrelevant. */
    if (n < 6U) {
        return (seq_t)0;
    }

    /* Initial values a(6) .. a(13). */
    static const seq_t init[8] = {15, 21, 28, 39, 52, 68, 88, 111};

    if (n <= 13U) {
        return init[n - 6U];
    }

    /* Ring buffer holding the last 8 terms:
       buf[0] = a(n-8), buf[1] = a(n-7), ..., buf[7] = a(n-1)   */
    seq_t buf[8];
    for (idx_t i = 0U; i < 8U; ++i) {
        buf[i] = init[i];
    }

    /* Iterate from n = 14 up to the requested n. */
    for (idx_t i = 14U; i <= n; ++i) {
        /* Compute a(i) using the recurrence. */
        seq_t next = 3 * buf[7U]
                   - 3 * buf[6U]
                   +     buf[5U]
                   /* + 0 * buf[4U] */ /* skipped */
                   +     buf[3U]
                   - 3 * buf[2U]
                   + 3 * buf[1U]
                   -     buf[0U];

        /* Shift the buffer: drop the oldest entry and insert the new one. */
        buf[0U] = buf[1U];
        buf[1U] = buf[2U];
        buf[2U] = buf[3U];
        buf[3U] = buf[4U];
        buf[4U] = buf[5U];
        buf[5U] = buf[6U];
        buf[6U] = buf[7U];
        buf[7U] = next;
    }

    return buf[7U];   /* a(n) */
}
