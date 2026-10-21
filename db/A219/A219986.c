/*
 * A219986  Number of n-step paths on a quartic lattice that move from
 *          (0,0,0,0) to (1,0,0,1) allowing all moves in {-1,0,1}^4 inclusive
 *          the zero move.
 * https://oeis.org/A219986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = ( coeff_{x^n}(1 + x + x^2)^n * coeff_{x^{n+1}}(1 + x +
 *            x^2)^n )^2 * where coeff_{x^k}(1 + x + x^2)^n is the trinomial
 *            coefficient T(n,k). * The computation uses the multiplicative
 *            recurrence * T(n,n) = Σ_{a=0}^{⌊n/2⌋} n! / (a! a! (n-2a)!) *
 *            T(n,n+1) = Σ_{a=1}^{⌊(n+1)/2⌋} n! / ((a-1)! a!...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* --- compute T(n,n) --- */
    idx_t a_max0 = n / 2;          /* floor(n/2) */
    seq_t sum0 = 1;                /* term for a = 0 */
    seq_t term0 = 1;               /* current term for T(n,n) */

    for (idx_t a = 0; a < a_max0; ++a) {
        /* term for a+1 */
        seq_t num = (seq_t)(n - 2*a) * (seq_t)(n - 2*a - 1);
        seq_t den = (seq_t)(a + 1) * (seq_t)(a + 1);
        term0 = term0 * num / den;
        sum0 += term0;
    }

    /* --- compute T(n,n+1) --- */
    seq_t sum1 = 0;                /* sum of terms for a = 1 .. floor((n+1)/2) */
    seq_t term1 = 0;               /* current term for T(n,n+1) */

    if (n > 0) {
        term1 = (seq_t)n;          /* a = 1 term */
        sum1 = term1;
        idx_t a_max1 = (n + 1) / 2;   /* floor((n+1)/2) */

        for (idx_t a = 1; a < a_max1; ++a) {
            /* term for a+1 */
            seq_t num = (seq_t)(n + 1 - 2*a) * (seq_t)(n - 2*a);
            seq_t den = (seq_t)a * (seq_t)(a + 1);
            term1 = term1 * num / den;
            sum1 += term1;
        }
    }

    /* --- combine and return --- */
    seq_t prod = sum0 * sum1;
    return prod * prod;
}
