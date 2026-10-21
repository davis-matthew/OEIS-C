/*
 * A219509  Pierce expansion of 40 - 16*sqrt(6).
 * https://oeis.org/A219509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A219509: Pierce expansion of 40 - 16*sqrt(6)
   Formula: a(0) = 1, a(1) = 5
   Recurrence:
     for n >= 2,
        if n is even:   a(n) = 4*(a(n-1) + 1)
        if n is odd:    a(n) = 2*(a(n-2))^2 - 1
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)5;

    /* Keep the two most recent values needed for the recurrence. */
    seq_t a_prev2 = (seq_t)1;   /* a(0)   */
    seq_t a_prev1 = (seq_t)5;   /* a(1)   */
    seq_t a_curr  = 0;           /* placeholder for a(n) */

    for (idx_t i = 2u; i <= n; ++i)
    {
        if ((i & 1u) == 0u)                /* even index */
        {
            /* a(i) = 4 * (a(i-1) + 1) */
            a_curr = a_prev1 + (seq_t)1;
            a_curr = a_curr << 2;          /* multiply by 4 */
        }
        else                                 /* odd index */
        {
            /* a(i) = 2 * (a(i-2))^2 - 1 */
            seq_t t = a_prev2 * a_prev2;    /* (a(i-2))^2 */
            a_curr = t + t;                 /* 2 * t */
            a_curr -= (seq_t)1;             /* -1 */
        }

        /* Shift window for next iteration */
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr;
}
