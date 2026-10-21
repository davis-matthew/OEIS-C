/*
 * A077936  Expansion of 1/(1 - 2*x - 2*x^2 - x^3).
 * https://oeis.org/A077936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A077936
   Formula: a(0)=1, a(1)=2, a(2)=6,
            a(n)=2*a(n-1)+2*a(n-2)+a(n-3)  for n>=3
   Expands 1/(1-2x-2x^2-x^3). */

#include <stddef.h>   /* for NULL */

seq_t a(idx_t n)
{
    /* Handle the first three initial terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;

    /* Iteratively compute using the recurrence.
       Keep the last three values to avoid extra storage. */
    seq_t a_prev3 = (seq_t)1;   /* a(n-3) */
    seq_t a_prev2 = (seq_t)2;   /* a(n-2) */
    seq_t a_prev1 = (seq_t)6;   /* a(n-1) */
    seq_t a_curr  = 0;
    for (idx_t i = 3; i <= n; ++i) {
        a_curr = (seq_t)2*a_prev1 + (seq_t)2*a_prev2 + a_prev3;
        a_prev3 = a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}
