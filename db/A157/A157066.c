/*
 * A157066  Number of integer sequences of length n+1 with sum zero and sum of
 *          absolute values 34.
 * https://oeis.org/A157066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=1..min(n,17)} binomial(n+1,i) * binomial(16,i-1) *
 *            binomial(n-i+17,17) OEIS A157066: Number of integer sequences of
 *            length n+1 with sum zero and sum of absolute values 34
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 23, u128 n <= 96,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    idx_t limit = n < 17 ? n : 17;   /* binomial(16,i-1) is 0 for i>17 */
    seq_t result = 0;
    seq_t c1 = 1;                    /* Current binomial(n+1, i)   */

    for (idx_t i = 1; i <= limit; ++i)
    {
        /* Update C(n+1, i) using the recurrence */
        c1 = c1 * (n + 2 - i) / i;

        /* C(16, i-1) – small value; compute directly  */
        seq_t c2 = binomial(16, i - 1);

        /* C(n-i+17, 17) – compute with helper */
        seq_t c3 = binomial(n - i + 17, 17);

        /* Add the term to the result */
        seq_t term = c1 * c2 * c3;
        result += term;
    }

    return result;
}
