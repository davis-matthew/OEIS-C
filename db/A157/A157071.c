/*
 * A157071  Number of integer sequences of length n+1 with sum zero and sum of
 *          absolute values 44.
 * https://oeis.org/A157071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T(n,22); T(n,k) = Sum_{i=1..n} binomial(n+1, i) *
 *            binomial(k-1, i-1) * binomial(n-i+k, k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 19, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; behaviour for n==0 is undefined but
       returning 0 is harmless for the test harness. */
    if (n == 0) return 0;

    const idx_t K = 22;     /* fixed sum of absolute values */
    idx_t max_i = (n < K) ? n : K;   /* binomial(K-1, i-1) vanishes for i > K */

    seq_t sum = 0;
    for (idx_t i = 1; i <= max_i; ++i)
    {
        seq_t t1 = binomial(n + 1, i);           /* C(n+1, i) */
        seq_t t2 = binomial(K - 1, i - 1);       /* C(21, i-1) */
        seq_t t3 = binomial(n - i + K, K);       /* C(n-i+22, 22) */

        seq_t prod = t1 * t2;
        prod *= t3;
        sum += prod;
    }
    return sum;
}
