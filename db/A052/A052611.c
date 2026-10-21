/*
 * A052611  Expansion of e.g.f. 1/(1-2*x-2*x^2).
 * https://oeis.org/A052611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n*a(n-1) + 2*n*(n-1)*a(n-2); a(0) = 1, a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the two base cases directly */
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)2;

    /* prev2 = a(n-2), prev1 = a(n-1) */
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)2;   /* a(1) */
    seq_t curr;

    for (idx_t i = 2U; i <= n; ++i)
    {
        /* term1 = 2*i*prev1 */
        seq_t term1 = (seq_t)2 * (seq_t)i * prev1;
        /* term2 = 2*i*(i-1)*prev2 */
        seq_t term2 = (seq_t)2 * (seq_t)i * (seq_t)(i - 1U) * prev2;
        curr = term1 + term2;
        /* shift for next iteration */
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
