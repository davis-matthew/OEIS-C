/*
 * A165149  a(n) = (3*9^n - 5^n)/2.
 * https://oeis.org/A165149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - 45*a(n-2), a(0)=1, a(1)=11
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;

    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)11;  /* a(1) */
    seq_t a_curr;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* compute a_curr = 14*a_prev1 - 45*a_prev2 using only
           unlimited-precision operations that are valid for all
           seq_t types (including arbitrary‑precision big integers). */
        a_curr = a_prev1 * (seq_t)14 - a_prev2 * (seq_t)45;

        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_prev1;
}
