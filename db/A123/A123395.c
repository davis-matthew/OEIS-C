/*
 * A123395  Values X satisfying the equation 7(X-Y)^4-16XY=0, where X>=Y.
 * https://oeis.org/A123395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 0
   a(1) = 27
   a(2) = 6144
   a(3) = 1549125
   a(n) = 270*a(n-1) - 4066*a(n-2) + 270*a(n-3) - a(n-4)  for n ≥ 4
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* short‑circuit the few initial values */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)27;
    if (n == 2) return (seq_t)6144;
    if (n == 3) return (seq_t)1549125;

    /* four successive terms that will be updated in the loop */
    seq_t a0 = (seq_t)0;          /* a(n-4) */
    seq_t a1 = (seq_t)27;         /* a(n-3) */
    seq_t a2 = (seq_t)6144;       /* a(n-2) */
    seq_t a3 = (seq_t)1549125;    /* a(n-1) */
    seq_t an;                     /* a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute the next term according to the recurrence */
        an = (a3 * (seq_t)270)          /* 270*a(n-1) */
           + (a1 * (seq_t)270)          /* 270*a(n-3) */
           - (a2 * (seq_t)4066)         /* 4066*a(n-2) */
           - a0;                       /* a(n-4) */

        /* Slide the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return a3;   /* a(n) is now stored in a3 after the loop */
}
