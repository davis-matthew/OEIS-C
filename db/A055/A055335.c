/*
 * A055335  Number of asymmetric (identity) trees with n nodes and 4 leaves.
 * https://oeis.org/A055335
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-225 -762*n +516*n^2 -100*n^3 +6*n^4)/1152
 *            -(3/128)*(-1)^n*(2*n-11) -(1/16)*(2 -(-1)^n)*(-1)^binomial(n,2)
 *            -(1/9)*ChebyshevU(n-1, -1/2) where ChebyshevU(k, -1/2) is
 *            periodic with period 6: [1, -1, 0, 1, -1, 0, …] for k =
 *            0,1,2,3,4,5,…
 * Range:     u8 n <= 19, u16 n <= 63, u32 n <= 957, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid overflow for n up to about 10^4. */
    __int128 N = (uint64_t)n;                    /* n as signed integer      */
    __int128 base_num = -225 - 762*N + 516*N*N   /* numerator of the polynomial part  */
                           - 100*N*N*N
                           + 6*N*N*N*N;

    /* (-1)^n : +1 for even n, -1 for odd n. */
    int sgn_n = ((n & 1) == 0) ? 1 : -1;

    /* term1 : (3/128)*(-1)^n*(2*n-11)  -> scaled by 1152 => 27*sgn_n*(2*n-11) */
    __int128 term1 = 27 * sgn_n * (2*N - 11);

    /* term2 : (1/16)*(2 - (-1)^n)*(-1)^C  -> scaled by 1152 => 72*(2 -sgn_n)*sgn_C */
    int two_minus_sgn_n = 2 - sgn_n;                   /* 1 or 3 */
    __int128 binom = (N*(N-1))/2;                     /* C = n*(n-1)/2   */
    int sgn_binom = ((binom & 1) == 0) ? 1 : -1;       /* (-1)^C          */
    __int128 term2 = 72 * two_minus_sgn_n * sgn_binom;

    /* term3 : (1/9)*ChebyshevU(n-1, -1/2) -> scaled by 1152 => 128*U_{n-1} */
    static const int cheb[6] = {1, -1, 0, 1, -1, 0};
    int u = cheb[(int)((n-1)%6)];                    /* U_{n-1} for x = -1/2 */
    __int128 term3 = 128 * u;

    /* total numerator scaled by 1152 */
    __int128 numer = base_num - term1 - term2 - term3;

    /* final result */
    return (seq_t)(numer / 1152);
}
