/*
 * A037238  5x - 1 sequence starting at 19 (a(n+1) = a(n)/2 if a(n) is even,
 *          or 5*a(n)-1 if a(n) is odd).
 * https://oeis.org/A037238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(1) = 19; a(n+1) = a(n)/2 if a(n) is even, a(n+1) = 5*a(n)-1 if
 *            a(n) is odd. The sequence enters the cycle 4,2,1 after 437 steps
 * Range:     u8 n <= 5, u16 n <= 27, u32 n <= 437, u64 n <= 437,
 *            u128 n <= 437, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* The sequence can be generated iteratively; memoisation is
       unnecessary for n up to at least 10000. */
    seq_t value = (seq_t)19;        /* a(1) */
    if (n <= 1) {
        return value;
    }

    /* Iterate from 2 to n, updating value each step. */
    for (idx_t i = 2; i <= n; ++i) {
        if ((value & (seq_t)1) == 0) {
            value = value / 2;               /* even case */
        } else {
            value = value * (seq_t)5 - (seq_t)1; /* odd case */
        }
    }

    return value;
}
