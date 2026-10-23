/*
 * A157058  Number of integer sequences of length n+1 with sum zero and sum of
 *          absolute values 18.
 * https://oeis.org/A157058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1}^{min(n,9)} C(n+1,i) * C(8,i-1) * C(n-i+9,9)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 12, u64 n <= 47, u128 n <= 572,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          where C(a,b) is the binomial coefficient.                       */
/* This implements the combinatorial formula for OEIS sequence A157058.      */

#include <stdint.h>

/* Small binomial routine for very small k (k <= 9). Returns C(N,K). */
static seq_t binom_small(idx_t N, idx_t K)
{
    if (K == 0 || K == N)
        return (seq_t)1;
    if (K > N)
        return (seq_t)0;
    if (K > N - K)
        K = N - K;

    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= K; ++i) {
        res = res * (N - (K - i)); /* multiply by numerator */
        res = res / i;             /* divide by denominator */
    }
    return res;
}

/* Sequence A157058: number of integer sequences of length n+1 with sum zero
   and sum of absolute values 18. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* undefined below offset; returns 0 */

    const idx_t max_i = (n < 9) ? n : 9;
    static const seq_t binom8[9] = {
        (seq_t)1, (seq_t)8, (seq_t)28, (seq_t)56, (seq_t)70,
        (seq_t)56, (seq_t)28, (seq_t)8,  (seq_t)1
    };

    seq_t sum = (seq_t)0;
    for (idx_t i = 1; i <= max_i; ++i) {
        seq_t c1 = binom_small(n + 1, i);
        seq_t c2 = binom8[i - 1];
        seq_t c3 = binom_small(n - i + 9, 9);

        seq_t tmp = c1 * c2;
        tmp = tmp * c3;
        sum += tmp;
    }
    return sum;
}
