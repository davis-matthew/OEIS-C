/*
 * A176233  Determinant of n X n matrix with rows (n^2,-1,0,...,0), (1,n^2,-1,
 *          0,...,0), (0,1,n^2,-1,0,...,0), ...,(0,0,...,1,n^2).
 * https://oeis.org/A176233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = determinant of the n×n tridiagonal matrix with main
 *            diagonal n², subdiagonal 1, and superdiagonal -1. The
 *            determinant Dₙ satisfies D₀ = 1, D₁ = n², Dₖ = n² · Dₖ₋₁ + Dₖ₋₂
 *            (k ≥ 2). Hence a(n) = Dₙ
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n<1, but safe */

    seq_t s = (seq_t)n * (seq_t)n; /* n² */

    seq_t d_prev2 = 1;            /* D₀ */
    if (n == 1)
        return s;                  /* D₁ = n² */

    seq_t d_prev1 = s;            /* D₁ */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t d_curr = s * d_prev1 + d_prev2;
        d_prev2 = d_prev1;
        d_prev1 = d_curr;
    }
    return d_prev1;                /* Dₙ */
}
