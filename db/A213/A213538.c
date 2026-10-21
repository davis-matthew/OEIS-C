/*
 * A213538  Maximum deviation from n in Collatz trajectory of n.
 * https://oeis.org/A213538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum absolute difference |x - n| over all terms x in
 *            the Collatz trajectory of n (starting from n and ending at 1)
 * Range:     u8 n <= 26, u16 n <= 702, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation directly simulates the Collatz trajectory for each n
   and keeps track of the largest deviation observed. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Convert index to the sequence type once */
    seq_t start = (seq_t)n;
    seq_t cur    = start;
    seq_t maxdev = 0;

    while (1) {
        /* Compute absolute difference |cur - start| */
        seq_t diff = (cur >= start) ? (cur - start) : (start - cur);
        if (diff > maxdev) maxdev = diff;

        if (cur == 1)            /* terminate at the end of the trajectory */
            break;

        /* Next Collatz step: if even, divide by 2; otherwise multiply by 3 and add 1 */
        if ((cur & 1) == 0)
            cur = cur >> 1;      /* cur / 2 */
        else
            cur = cur * 3 + 1;
    }

    return maxdev;
}
