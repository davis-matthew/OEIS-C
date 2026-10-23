/*
 * A190419  Number of (n+2)X(n+2) symmetric binary matrices without the
 *          pattern 1 1 1 diagonally.
 * https://oeis.org/A190419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \prod_{i=1}^{n+2} g(i) where g(i) is the number of binary
 *            strings of length i with no substring "111". g(i) satisfies the
 *            Tribonacci recurrence: g(0)=1, g(1)=2, g(2)=4,
 *            g(i)=g(i-1)+g(i-2)+g(i-3) for i≥3. Hence a(1)=56, a(2)=728, ...
 *            as given in the OEIS entry
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t m = n + 2;         /* length of the longest diagonal */

    /* Tribonacci values for g(0), g(1), g(2) */
    seq_t g0 = 1;            /* g(0) */
    seq_t g1 = 2;            /* g(1) */
    seq_t g2 = 4;            /* g(2) */

    seq_t result = 1;        /* product accumulator */

    /* accumulate g(1) and g(2) if needed */
    if (m >= 1) result = result * g1;
    if (m >= 2) result = result * g2;

    /* sliding window for Tribonacci recurrence */
    seq_t prev3 = g0;        /* g(i-3)  */
    seq_t prev2 = g1;        /* g(i-2)  */
    seq_t prev1 = g2;        /* g(i-1)  */

    for (idx_t i = 3; i <= m; ++i) {
        seq_t gi = prev1 + prev2 + prev3;  /* g(i) */
        result = result * gi;              /* multiply into the product */
        /* shift window */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = gi;
    }

    return result;
}
