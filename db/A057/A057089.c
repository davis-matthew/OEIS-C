/*
 * A057089  Scaled Chebyshev U-polynomials evaluated at i*sqrt(6)/2.
 *          Generalized Fibonacci sequence.
 * https://oeis.org/A057089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 6*a(n-2); a(0)=1, a(1)=6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 6;

    seq_t prev2 = 1;  /* a(0) */
    seq_t prev1 = 6;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = (seq_t)(6 * (prev1 + prev2));
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
