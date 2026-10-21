/*
 * A205795  Sums of coefficients of polynomials from 5n-th moments of X ~
 *          Hypergeometric(4m, 5m, m).
 * https://oeis.org/A205795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 24, a(n) = 5 * (5n-6) * (5n-7) * (5n-8) * (5n-9) * a(n-1)
 *            for n ≥ 2 (which is equivalent to the recurrence
 *            a(n)=120*A151989(n‑2)*a(n‑1) with A151989(k)=C(5k+4,4))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)               /* out‑of‑offset index – undefined; return 0 */
        return (seq_t)0;

    seq_t res = (seq_t)24;    /* a(1) */

    for (idx_t k = 2; k <= n; ++k) {
        /* N = 5*k - 6 ; all calculations in seq_t to avoid truncation */
        seq_t N = (seq_t)5 * (seq_t)k - 6;
        /* factor = 5 * N * (N-1) * (N-2) * (N-3) */
        seq_t factor = (seq_t)5 * N * (N - 1) * (N - 2) * (N - 3);
        res *= factor;
    }

    return res;
}
