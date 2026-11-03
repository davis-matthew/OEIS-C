/*
 * A093196  Number of 4 X 4 pandiagonal magic squares with line sum 2*n.
 * https://oeis.org/A093196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{k=0}^{5} coeff_k * binomial(n+6-k, 6) where coeff =
 *            [1,21,90,90,21,1] (polynomial (1+x)(1+20x+70x²+20x³+x⁴)
 *            coefficients)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes binomial(N,6) for N ≥ 0, returns 0 if N<6. */
static seq_t binom6(idx_t N)
{
    if (N < 6) return 0;
    unsigned __int128 tmp = 1;
    /* compute (N-5)*(N-4)*…*N / 6!  */
    tmp = tmp * (N - 5); tmp /= 1;
    tmp = tmp * (N - 4); tmp /= 2;
    tmp = tmp * (N - 3); tmp /= 3;
    tmp = tmp * (N - 2); tmp /= 4;
    tmp = tmp * (N - 1); tmp /= 5;
    tmp = tmp * (N);     tmp /= 6;
    return (seq_t)tmp;
}

seq_t a(idx_t n)
{
    static const uint64_t coeff[6] = {1, 21, 90, 90, 21, 1};
    seq_t result = 0;
    for (int k = 0; k < 6; ++k) {
        idx_t N = n + 6 - k;    /* N = n+6-k */
        seq_t term = binom6(N);
        result += term * (seq_t)coeff[k];
    }
    return result;
}
