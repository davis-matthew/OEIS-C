/*
 * A055612  a(n) = Product_{m=1..n} (binomial(n,m)+1).
 * https://oeis.org/A055612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{m=1..n} (binomial(n,m)+1) (OEIS A055612)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle offset: a(0) = 1 (empty product) */
    if (n == 0)
        return 1;

    seq_t result = 1;   /* running product of (binom+1)  */
    seq_t binom   = 1;   /* C(n,0) starts at 1          */

    for (idx_t m = 1; m <= n; ++m) {
        /* Compute C(n,m) from C(n,m-1) via
           binom = binom * (n-m+1) / m   (exact division)          */
        seq_t numerator   = (seq_t)(n - m + 1);
        seq_t denominator = (seq_t)m;
        binom = binom * numerator / denominator;

        /* Multiply into the accumulating product   */
        result = result * (binom + 1);
    }

    return result;
}
