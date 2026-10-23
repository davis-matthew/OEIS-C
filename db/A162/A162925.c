/*
 * A162925  Number of reduced words of length n in Coxeter group on 5
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A162925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A162925: Number of reduced words of length n in the Coxeter
   group on 5 generators with relations (S_i)^2 = (S_i S_j)^4 = I.
   The generating function is
       (t^4 + 2*t^3 + 2*t^2 + 2*t + 1)/(6*t^4 - 3*t^3 - 3*t^2 - 3*t + 1).
   This satisfies the linear recurrence
       a(0)=1,  a(1)=5,  a(2)=20,  a(3)=80,
       a(4)=1 + 3 a(3) + 3 a(2) + 3 a(1) - 6 a(0),
       a(n) = 3 a(n-1) + 3 a(n-2) + 3 a(n-3) - 6 a(n-4)   for n >= 5.
*/

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)20;
    if (n == 3) return (seq_t)80;

    /* Compute a4 using the exceptional formula for n = 4 */
    seq_t a0 = (seq_t)1, a1 = (seq_t)5, a2 = (seq_t)20, a3 = (seq_t)80;
    seq_t a4 = (seq_t)1 + (seq_t)3 * a3 + (seq_t)3 * a2 + (seq_t)3 * a1
               - (seq_t)6 * a0;
    if (n == 4) return a4;

    /* For n >= 5, iterate using the recurrence */
    seq_t prev4 = a1;   /* a(n-4) */
    seq_t prev3 = a2;   /* a(n-3) */
    seq_t prev2 = a3;   /* a(n-2) */
    seq_t prev1 = a4;   /* a(n-1) */
    seq_t next;
    for (idx_t i = 5; i <= n; ++i) {
        next = (seq_t)3 * prev1 + (seq_t)3 * prev2 +
               (seq_t)3 * prev3 - (seq_t)6 * prev4;

        /* shift the window of previous terms */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;
}
