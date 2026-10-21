/*
 * A096750  Expansion of (1-x+x^2)/(1-2x+2x^2-x^3-x^4).
 * https://oeis.org/A096750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 63, u64 n <= 125,
 *            u128 n <= 248, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*a(n-1) - 2*a(n-2) + a(n-3) + a(n-4)
 * with initial values a(0) = a(1) = a(2) = a(3) = 1
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For n < 4 the value is always 1. */
    if (n < 4)
        return (seq_t)1;

    /* Maintain the last four terms of the sequence. */
    seq_t buf[4] = {(seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1};

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute a(i) using the recurrence. */
        seq_t next = 2 * buf[3] - 2 * buf[2] + buf[1] + buf[0];

        /* Shift the window: buf[0] <- buf[1], ..., buf[3] <- next. */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = next;
    }

    return buf[3]; /* a(n) is stored in buf[3] after the loop */
}
