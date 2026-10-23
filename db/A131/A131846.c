/*
 * A131846  Expansion of series reversion of x*(1-6*x)/(1-x).
 * https://oeis.org/A131846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth term of A131846 Recurrence: for n ≥ 3 n*a(n) =
 *            11*(2*n-3)*a(n-1) – (n-3)*a(n-2) Initial values: a(1) = 1, a(2)
 *            = 5 This implementation uses the recurrence, computing only two
 *            previous terms at a time, producing the exact result for all n ≥
 *            1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* not used, but safe */

    if (n == 1)
        return (seq_t)1;

    if (n == 2)
        return (seq_t)5;

    seq_t prev1 = (seq_t)5;    /* a(n-1) for n=2 */
    seq_t prev2 = (seq_t)1;    /* a(n-2) for n=2 */

    for (idx_t i = 3; i <= n; ++i) {
        /* Compute numerator: 11*(2*i-3)*prev1 - (i-3)*prev2 */
        seq_t term1 = (seq_t)(2*i - 3);   /* (2*i-3) as seq_t */
        term1 *= (seq_t)11;              /* 11*(2*i-3) */
        term1 *= prev1;                  /* 11*(2*i-3)*prev1 */

        seq_t term2 = (seq_t)(i - 3);    /* (i-3) as seq_t */
        term2 *= prev2;                  /* (i-3)*prev2 */

        seq_t numerator = term1 - term2; /* exact integer */
        seq_t cur = numerator / (seq_t)i; /* division is exact by recurrence */

        /* Shift for next iteration */
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1; /* a(n) */
}
