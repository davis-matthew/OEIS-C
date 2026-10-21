/*
 * A226307  a(n) = Sum_{k >= 0}
 *          binomial(6*n,k)*binomial(3*n,k)*binomial(2*n,k).
 * https://oeis.org/A226307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(6*n, k)*binomial(3*n,
 *            k)*binomial(2*n, k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t maxk = n;
    seq_t sum = 0;
    seq_t c6 = 1, c3 = 1, c2 = 1;   /* Initial binomial values for k = 0 */

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t prod = c6;
        prod *= c3;
        prod *= c2;
        sum += prod;

        if (k == maxk) {
            break;   /* No further updates needed */
        }

        idx_t denom = k + 1;
        c6 = c6 * (seq_t)(6 * n - k) / (seq_t)denom;
        c3 = c3 * (seq_t)(3 * n - k) / (seq_t)denom;
        c2 = c2 * (seq_t)(2 * n - k) / (seq_t)denom;
    }

    return sum;
}
