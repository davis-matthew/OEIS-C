/*
 * A138829  Bisection of imperfect numbers A132999.
 * https://oeis.org/A138829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS sequence A138829: Bisection of imperfect numbers A132999
   Recurrence (derived from generating function):
      a(1) = 1
      a(2) = 3
      a(n) = 2*a(n-1) - a(n-2)              for n > 2
      with corrections:
         +1 at n = 4 and n = 14
         -1 at n = 5 and n = 15
   (The adjustments correspond to the non‑zero terms in the numerator
    of the generating function.) 
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;      /* undefined for n < 1, return 0 just to compile */

    if (n == 1)
        return 1;
    if (n == 2)
        return 3;

    seq_t prev = 1;    /* a(1) */
    seq_t curr = 3;    /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        /* recurrence: a(i) = 2*a(i-1) - a(i-2) */
        seq_t next = curr + (curr - prev);  /* safe: prev <= curr */

        /* apply the finite corrections from the generating function */
        if (i == 4 || i == 14)
            next++;          /* +1 */
        else if (i == 5 || i == 15)
            next--;          /* -1 */

        prev = curr;
        curr = next;
    }

    return curr;
}
