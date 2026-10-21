/*
 * A059358  Coefficients in expansion of Sum_{n >= 1} x^n/(1-x^n)^4.
 * https://oeis.org/A059358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (sigma_3(n) + 3*sigma_2(n) + 2*sigma_1(n)) / 6
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where sigma_k(n) is the sum of the k-th powers of all divisors of n. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t sigma1 = 0;   /* Σ d              */
    seq_t sigma2 = 0;   /* Σ d^2            */
    seq_t sigma3 = 0;   /* Σ d^3            */

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            seq_t val = (seq_t)d;
            sigma1 += val;
            sigma2 += val * val;
            sigma3 += val * val * val;

            idx_t q = n / d;
            if (q != d) {
                seq_t val2 = (seq_t)q;
                sigma1 += val2;
                sigma2 += val2 * val2;
                sigma3 += val2 * val2 * val2;
            }
        }
    }

    seq_t result = (sigma3 + 3 * sigma2 + 2 * sigma1) / 6;
    return result;
}
