/*
 * A068022  Z(S_m; sigma[1](n), sigma[2](n),..., sigma[m](n)) where Z(S_m;
 *          x_1,x_2,...,x_m) is the cycle index of the symmetric group S_m and
 *          sigma[k](n) is the sum of k-th powers of divisors of n; m=5.
 * https://oeis.org/A068022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/5! * ( sigma1^5 + 10*sigma1^3*sigma2 +
 *            20*sigma1^2*sigma3 + 15*sigma1*sigma2^2 + 30*sigma1*sigma4 +
 *            20*sigma2*sigma3 + 24*sigma5 ) where sigma_k is the sum of the
 *            k‑th powers of the divisors of n. The function works for any n ≥
 *            1 and is O(√n) per call
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* sigma[1] … sigma[5] are stored at indices 1…5. */
    seq_t sigma[6] = {0, 0, 0, 0, 0, 0};

    /* Enumerate all divisors d ≤ sqrt(n).  For each divisor e = n / d
       we add d^k and, if d != e, add e^k.  Powers are built by
       repeated multiplication to avoid floating types. */
    for (idx_t d = 1; d * d <= n; ++d) if (n % d == 0) {
        idx_t e = n / d;

        seq_t d1 = (seq_t)d;
        seq_t d2 = d1 * d1;
        seq_t d3 = d2 * d1;
        seq_t d4 = d3 * d1;
        seq_t d5 = d4 * d1;

        seq_t e1 = (seq_t)e;
        seq_t e2 = e1 * e1;
        seq_t e3 = e2 * e1;
        seq_t e4 = e3 * e1;
        seq_t e5 = e4 * e1;

        sigma[1] += d1;
        sigma[2] += d2;
        sigma[3] += d3;
        sigma[4] += d4;
        sigma[5] += d5;

        if (d != e) {
            sigma[1] += e1;
            sigma[2] += e2;
            sigma[3] += e3;
            sigma[4] += e4;
            sigma[5] += e5;
        }
    }

    /* Compute the polynomial expression term by term. */
    seq_t term1 = sigma[1] * sigma[1] * sigma[1] * sigma[1] * sigma[1];   /* sigma1^5 */
    seq_t term2 = 10 * sigma[1] * sigma[1] * sigma[1] * sigma[2];           /* 10*sigma1^3*sigma2 */
    seq_t term3 = 20 * sigma[1] * sigma[1] * sigma[3];                     /* 20*sigma1^2*sigma3 */
    seq_t term4 = 15 * sigma[1] * sigma[2] * sigma[2];                     /* 15*sigma1*sigma2^2 */
    seq_t term5 = 30 * sigma[1] * sigma[4];                                /* 30*sigma1*sigma4 */
    seq_t term6 = 20 * sigma[2] * sigma[3];                                /* 20*sigma2*sigma3 */
    seq_t term7 = 24 * sigma[5];                                           /* 24*sigma5 */

    seq_t sum = term1 + term2 + term3 + term4 + term5 + term6 + term7;

    /* Divide by 5! = 120.  Integer division is exact by the
       theorem that the cycle index yields an integer. */
    return sum / 120;
}
