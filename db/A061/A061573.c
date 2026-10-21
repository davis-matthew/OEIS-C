/*
 * A061573  a(n) = (n!)^2*Sum_{k=1..n} 1/k!.
 * https://oeis.org/A061573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)/2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1, a(2) = 6,
                a(n) = n*(n+1)*a(n-1) - n*(n-1)^2*a(n-2) for n >= 3 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* behaviour for n < 1 is undefined, return 0 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;

    seq_t a_prev2 = (seq_t)1;   /* a(1) */
    seq_t a_prev1 = (seq_t)6;   /* a(2) */
    seq_t a_curr  = 0;

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t si   = (seq_t)i;
        seq_t siplus1 = si + (seq_t)1;
        seq_t siminus1 = si - (seq_t)1;
        seq_t term1 = si * siplus1 * a_prev1;
        seq_t term2 = si * siminus1 * siminus1 * a_prev2;
        a_curr = term1 - term2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}
