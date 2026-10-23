/*
 * A198256  Row sums of A197653.
 * https://oeis.org/A198256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+1, k)^2 * binomial(n, k)^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k;
    seq_t sum = 0;
    seq_t bin_nk = 1;      /* C(n,0)   */
    seq_t bin_np1k = 1;    /* C(n+1,0) */

    for (k = 0; k <= n; ++k) {
        seq_t prod = bin_nk * bin_np1k;
        sum += prod * prod;               /* (C(n+1,k)*C(n,k))^2 */

        if (k == n) break;                /* last term processed */

        /* update binomial coefficients for next k */
        bin_nk    = (bin_nk * (seq_t)(n - k)) / (seq_t)(k + 1);    /* C(n,k+1)   */
        bin_np1k  = (bin_np1k * (seq_t)(n + 1 - k)) / (seq_t)(k + 1); /* C(n+1,k+1) */
    }
    return sum;
}
