/*
 * A100510  Bisection of A005425.
 * https://oeis.org/A100510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n+3)*a(n-1) - 2*(2*n-1)*(n-1)*a(n-2), a(0)=2, a(1)=14
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)14;
    seq_t prev2 = (seq_t)2;      /* a(0) */
    seq_t prev1 = (seq_t)14;     /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t coeff1 = (seq_t)(4 * i + 3);
        seq_t coeff2 = (seq_t)(2 * (2 * i - 1) * (i - 1));
        seq_t term1   = prev1 * coeff1;
        seq_t term2   = prev2 * coeff2;
        seq_t cur     = term1 - term2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
