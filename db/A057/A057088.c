/*
 * A057088  Scaled Chebyshev U-polynomials evaluated at i*sqrt(5)/2.
 *          Generalized Fibonacci sequence.
 * https://oeis.org/A057088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5 * (a(n-1) + a(n-2)), a(-1)=0, a(0)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)5;   /* a(1) */
    seq_t cur   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = ((seq_t)5) * (prev1 + prev2);
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
