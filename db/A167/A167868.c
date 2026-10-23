/*
 * A167868  a(n) = 3^n * Sum_{k=0..n} binomial(2*k,k)^3 / 3^k.
 * https://oeis.org/A167868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^n * Sum_{k=0..n} binomial(2*k,k)^3 / 3^k
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum  = 0;   /* accumulator for the sum */
    seq_t binom = 1;  /* binom(2*k,k), start with k=0 */
    seq_t pow3  = 1;  /* will hold 3^(n-k) during the loop */

    /* Compute 3^n once */
    for (idx_t i = 0; i < n; ++i) {
        pow3 *= 3;
    }

    /* Sum over k = 0 .. n */
    for (idx_t k = 0; k <= n; ++k) {
        /* binom^3 */
        seq_t binom_sq = binom * binom;
        seq_t binom_cub = binom_sq * binom;

        /* term = 3^(n-k) * binom(2k,k)^3 */
        seq_t term = pow3 * binom_cub;
        sum += term;

        if (k < n) {
            /* update binom to binom(2*(k+1),k+1)
               binom(2*(k+1),k+1) = binom * (4*(k+1)-2) / (k+1) */
            seq_t tmp = binom * (4 * (k + 1) - 2);
            binom = tmp / (k + 1);

            /* update pow3 to 3^(n-(k+1)) */
            pow3 /= 3;
        }
    }

    return sum;
}
