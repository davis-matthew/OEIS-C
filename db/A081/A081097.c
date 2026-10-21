/*
 * A081097  Numbers n such that n^2=
 *          (1/5)*(n+floor(sqrt(5)*n*floor(sqrt(5)*n))).
 * https://oeis.org/A081097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A081097
   Numbers n such that
   n^2 = (1/5)*(n + floor(sqrt(5)*n*floor(sqrt(5)*n))).

   Recurrence (valid for all n ≥ 1):
     a(1) = 1
     a(2) = 6
     a(3) = 23
     a(4) = 40
     a(4k)   = 2*a(4k-1) - a(4k-2)
     a(4k+1) = 7*a(4k)   - a(4k-2) - 1
     a(4k+2) = 7*a(4k+1) - a(4k)   - 1
     a(4k+3) = 4*a(4k+2) - (a(4k+1)+1)/2

   The code computes the terms on demand using only the three most
   recent values; no dynamic allocation or arbitrary‑precision helpers
   are required, so it works for any seq_t type supported by the build.
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* base values */
    if (n == 0) return (seq_t)0; /* undefined for n ≤ 0, return 0 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)23;
    if (n == 4) return (seq_t)40;

    /* prev1 = a(i-1), prev2 = a(i-2), prev3 = a(i-3) */
    seq_t prev1 = (seq_t)40;   /* a4 */
    seq_t prev2 = (seq_t)23;   /* a3 */
    seq_t prev3 = (seq_t)6;    /* a2 */

    /* iterate from i = 5 up to requested n */
    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t next;
        switch (i & 3)           /* i % 4 */
        {
        case 1: /* a(4k+1) */
            next = (seq_t)(7 * prev1 - prev3 - 1);
            break;
        case 2: /* a(4k+2) */
            next = (seq_t)(7 * prev1 - prev2 - 1);
            break;
        case 3: /* a(4k+3) */
            next = (seq_t)(4 * prev1 - ((prev2 + 1) / 2));
            break;
        case 0: /* a(4k)   */
            next = (seq_t)(2 * prev1 - prev2);
            break;
        }

        /* shift the window for the next step */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = next;
    }

    return prev1;
}
