/*
 * A227152  Nonnegative solutions of the Pell equation x^2 - 101*y^2 = +1.
 *          Solutions x = a(n).
 * https://oeis.org/A227152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 402*a(n-1) - a(n-2) with a(0) = 1, a(1) = 201, for all n
 *            ≥ 0
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)201;

    seq_t prev2 = (seq_t)1;     /* a(0) */
    seq_t prev1 = (seq_t)201;   /* a(1) */

    for (idx_t i = 2U; i <= n; ++i)
    {
        /* cur = 402*prev1 - prev2   (prev2 is a(n-2), prev1 is a(n-1)) */
        seq_t cur = (seq_t)402 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
