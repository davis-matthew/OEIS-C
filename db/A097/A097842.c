/*
 * A097842  Chebyshev polynomials S(n,123) + S(n-1,123) with Diophantine
 *          property.
 * https://oeis.org/A097842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 123*a(n-1) - a(n-2), a(0) = 1, a(1) = 124
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool, unused but requested */

seq_t a(idx_t n)
{
    /* Handle the two base cases directly. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)124;

    /* Iteratively compute the recurrence up to n.  The loop runs
       at most n-1 iterations, which is O(n) time and suitable for
       n up to about 10 000. */
    seq_t a_prev_prev = (seq_t)1;   /* a(0)   */
    seq_t a_prev = (seq_t)124;      /* a(1)   */
    seq_t a_curr = 0;               /* a(n)   */

    for (idx_t i = 2; i <= n; ++i) {
        a_curr = ((seq_t)123 * a_prev) - a_prev_prev;
        a_prev_prev = a_prev;
        a_prev = a_curr;
    }
    return a_curr;
}
