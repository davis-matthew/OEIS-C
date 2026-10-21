/*
 * A089671  a(n) = S3(n,3), where S3(n, t) = Sum_{k=0..n} k^t *(Sum_{j=0..k}
 *          binomial(n,j))^3.
 * https://oeis.org/A089671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k^3 * (Sum_{j=0..k} binomial(n,j))^3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h> /* for bool if needed, not used here */

seq_t a(idx_t n)
{
    seq_t answer = 0;     /* sum of all terms */
    seq_t sumS   = 0;     /* S_k = sum_{j=0..k} binomial(n,j) */
    seq_t bin    = (seq_t)1; /* binom(n,0) */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* Update binom(n,k) for current k (except k=0) */
        if (k > 0)
        {
            seq_t numer = (seq_t)(n - (k - 1)); /* n - (k-1) = n - k + 1 */
            seq_t denom = (seq_t)k;
            bin = (bin * numer) / denom; /* exact division */
        }

        /* Update cumulative binomial sum S_k */
        sumS += bin;

        /* Compute k^3 */
        seq_t kseq = (seq_t)k;
        seq_t k3 = kseq * kseq * kseq;

        /* Compute S_k^3 */
        seq_t s3 = sumS * sumS * sumS;

        /* Add term to answer */
        answer += k3 * s3;
    }

    return answer;
}
