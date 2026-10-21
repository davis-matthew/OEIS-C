/*
 * A237660  Consider the Collatz trajectory of n; if all terms except n and 1
 *          are even then a(n) = 0, otherwise a(n) is the last odd number
 *          before 1.
 * https://oeis.org/A237660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 9492, u32 n <= 9492, u64 n <= 9492,
 *            u128 n <= 9492, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A237660 – Collatz last odd before 1.  */
/* For n ≥ 1, follow the Collatz trajectory.
   If every intermediate term (excluding the starting n and the final 1)
   is even, return 0; otherwise return the last odd term before the
   terminating 1. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) /* undefined in OEIS, but defensive */
        return (seq_t)0;

    seq_t current = (seq_t)n;
    seq_t lastOdd = (seq_t)0;
    bool allEven = true;

    /* Special case n == 1 – no intermediate terms, so allEven stays true. */
    if (current == (seq_t)1)
        return (seq_t)0;

    while (current != (seq_t)1)
    {
        /* Compute next term of the Collatz sequence. */
        seq_t next;
        if (current & (seq_t)1)
            /* odd: 3*x + 1 */
            next = current * (seq_t)3 + (seq_t)1;
        else
            /* even: x / 2 */
            next = current / (seq_t)2;

        if (next == (seq_t)1)
            break;      /* reached terminating 1 – do not consider it */

        if (next & (seq_t)1) {   /* odd intermediate term */
            allEven = false;
            lastOdd = next;
        }

        current = next;
    }

    return allEven ? (seq_t)0 : lastOdd;
}
