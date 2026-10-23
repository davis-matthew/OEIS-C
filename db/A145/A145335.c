/*
 * A145335  Numbers n such that there exists x in N : (x+1)^3-x^3=43*n^2.
 * https://oeis.org/A145335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 7, a(2) = 235963,
   a(n+2) = 33710 * a(n+1) - a(n)   (for n ≥ 1) */
#include <stdbool.h>  /* for bool, not strictly needed but allowed */

seq_t a(idx_t n)
{
    /* Values for n < 1 are undefined; handles n == 1 and n == 2 directly. */
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)235963;

    /* Compute by iterating the linear recurrence.  Works for any order of calls. */
    seq_t prev2 = (seq_t)7;          /* a(1)   */
    seq_t prev  = (seq_t)235963;     /* a(2)   */
    seq_t curr = 0;                  /* placeholder */

    for (idx_t i = 3; i <= n; ++i)
    {
        curr = (seq_t)33710 * prev - prev2;  /* a(i) = 33710 * a(i-1) - a(i-2) */
        prev2 = prev;  /* shift window: a(i-2) becomes a(i-1) of next step */
        prev  = curr;  /* shift window: a(i-1) becomes a(i) of next step */
    }

    return curr;
}
