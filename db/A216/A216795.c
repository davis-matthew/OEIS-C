/*
 * A216795  a(n) = sum_{k=0..n} binomial(n,k)^4 * 3^k.
 * https://oeis.org/A216795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..n} binomial(n,k)^4 * 3^k
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum   = 0;   /* the resulting sum */
    seq_t bin   = 1;   /* binomial(n,0) = 1 */
    seq_t pow3  = 1;   /* 3^0 */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* bin^4 */
        seq_t bin2 = bin * bin;
        seq_t bin4 = bin2 * bin2;

        /* accumulate current term */
        sum += bin4 * pow3;

        /* end of the last iteration? */
        if (k == n)
            break;

        /* update binomial to binomial(n, k+1) */
        bin = bin * (seq_t)(n - k) / (seq_t)(k + 1);
        /* update 3^k to 3^(k+1) */
        pow3 *= 3;
    }
    return sum;
}
