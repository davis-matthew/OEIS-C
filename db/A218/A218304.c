/*
 * A218304  E.g.f. A(x) satisfies A( x/(exp(2*x)*cosh(2*x)) ) =
 *          exp(3*x)*cosh(3*x).
 * https://oeis.org/A218304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = (3/2) * Σ_{k=0..n} (2k+3)^(n-1) * binomial(n,k)
 *            for n > 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

# include "../utils/binomial.h"   /* provides seq_t binomial(idx_t, idx_t) */
# include "../utils/pow_seq.h"     /* provides seq_t pow_seq(seq_t, int) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* n-1 fits into 'int' for the allowed n (≤ 10000) */
    int exp = (int)(n - 1);

    seq_t sum   = 0;
    seq_t binom = binomial(n, 0);  /* binomial(n,0) = 1 */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t base = (seq_t)(2 * k + 3);
        seq_t pw   = pow_seq(base, exp);   /* (2k+3)^(n-1) */
        sum += binom * pw;

        /* Prepare binomial(n, k+1) for next iteration */
        if (k < n) {
            seq_t numerator   = binom * (seq_t)(n - k);
            seq_t denominator = (seq_t)(k + 1);
            binom = numerator / denominator;   /* exact division */
        }
    }

    /* a(n) = (3/2) * sum */
    seq_t result = sum * (seq_t)3;
    result = result / (seq_t)2;

    return result;
}
