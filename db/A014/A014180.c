/*
 * A014180  a(n) = Sum_{k = 0..n} binomial(n,k)^3*binomial(n+k,k)^2.
 * https://oeis.org/A014180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^3 * binomial(n+k,k)^2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A014180. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t binom_nk = 1;   /* C(n,0) */
    seq_t binom_npk = 1;  /* C(n+0,0) */

    for (idx_t k = 0; k <= n; ++k)
    {
        if (k > 0)
        {
            /* Update C(n,k) = C(n,k-1) * (n-k+1)/k   */
            seq_t num_nk   = (seq_t)(n - k + 1);
            seq_t denom_nk = (seq_t)k;
            binom_nk = (binom_nk * num_nk) / denom_nk;

            /* Update C(n+k,k) = C(n+k-1,k-1) * (n+k)/k   */
            seq_t num_npk   = (seq_t)(n + k);
            seq_t denom_npk = (seq_t)k;
            binom_npk = (binom_npk * num_npk) / denom_npk;
        }

        seq_t binom_nk_cub = binom_nk * binom_nk * binom_nk;
        seq_t binom_npk_sq = binom_npk * binom_npk;
        sum += binom_nk_cub * binom_npk_sq;
    }

    return sum;
}
