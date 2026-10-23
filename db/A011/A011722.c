/*
 * A011722  A binary m-sequence: expansion of reciprocal of x^9+x^4+1.
 * https://oeis.org/A011722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A011722:  Binary m-sequence from 1/(x^9 + x^4 + 1)
   Recurrence: a(n) = a(n-4) XOR a(n-9)  for n >= 9
   Initial values: a(0)=a(1)=...=a(7)=0, a(8)=1
   The sequence is 511‑periodic. */
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t period[511];
    static int built = 0;

    if (!built) {
        /* initialise the first 9 terms */
        period[0] = period[1] = period[2] = period[3] = period[4] =
        period[5] = period[6] = period[7] = (seq_t)0;
        period[8] = (seq_t)1;

        /* compute the rest of the period using the recurrence */
        for (idx_t i = 9; i < 511; ++i) {
            period[i] = period[i - 4] ^ period[i - 9];
        }
        built = 1;
    }

    /* use the periodicity: 511 = 2^9 - 1 */
    idx_t m = n % 511;
    return period[m];
}
