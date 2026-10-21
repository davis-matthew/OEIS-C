/*
 * A075871  Numbers k such that 13*k^2 + 1 is a square.
 * https://oeis.org/A075871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1298*a(n-1) - a(n-2), a(1)=0, a(2)=180
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first two known terms directly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)180;

    /* a_prev2 = a(n-2), a_prev1 = a(n-1) */
    seq_t a_prev2 = (seq_t)0;  /* a(1) */
    seq_t a_prev1 = (seq_t)180; /* a(2) */
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i) {
        /* Compute a(i) = 1298*a_prev1 - a_prev2.
           The operations are performed in seq_t; for the 64‑bit build
           this may overflow for large i, but those terms are outside
           the test range.  For arbitrary‑precision seq_t all values
           are exact. */
        ai = ((seq_t)1298) * a_prev1 - a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = ai;
    }

    return ai;
}
