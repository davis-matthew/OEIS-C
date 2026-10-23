/*
 * A193821  Triangular array: the fusion of polynomial sequences P and Q given
 *          by p(n,x)=(x+2)^n and q(n,x)=x^n+x^(n-1)+...+x+1.
 * https://oeis.org/A193821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   For the n-th term a(n), let (r,c) be the unique * pair with 0 ≤
 *            r and 0 ≤ c ≤ r such that n = r*(r+1)/2 + c. * Then a(0) = 1 and
 *            for r ≥ 1 * * Let k = r-1. If c ≥ k, a(n) = 3^k. * Otherwise *
 *            a(n) = Σ_{i=0}^{c} binom(k,i) * 2^{k-i}. * * This implements the
 *            fusion of the polynomial sequences * P(r...
 * Range:     u8 n <= 28, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2  */
    idx_t r = 0;
    while ((idx_t)r * (r + 1) / 2 <= n)
        ++r;
    r--;               /* now r satisfies the inequality */

    /* column index within this row */
    idx_t c = n - (idx_t)r * (r + 1) / 2;

    /* trivial case: the very first term */
    if (r == 0)
        return (seq_t)1;

    /* for r ≥ 1 we have k = r-1  */
    idx_t k = r - 1;

    /* if we are past the last coefficient of (x+2)^{k-1} 
       the sum equals 3^k */
    if (c >= k) {
        seq_t val = (seq_t)1;
        for (idx_t i = 0; i < k; ++i)
            val *= (seq_t)3;
        return val;
    }

    /* otherwise compute the partial sum of binomial terms */
    seq_t sum   = (seq_t)0;
    seq_t binom = (seq_t)1;          /* binom(k,0) */
    seq_t pow2  = (seq_t)1;          /* 2^k */
    for (idx_t i = 0; i < k; ++i)
        pow2 *= (seq_t)2;            /* pow2 = 2^k */

    for (idx_t i = 0; i <= c; ++i) {
        seq_t term = binom * pow2;
        sum += term;
        if (i == c)               /* last needed term reached */
            break;
        /* update binomial coefficient: binom(k,i+1) = binom(k,i)*(k-i)/(i+1) */
        binom = binom * (seq_t)(k - i) / (seq_t)(i + 1);
        /* update power of two: 2^{k-i-1} = 2^{k-i} / 2  */
        pow2 >>= 1;
    }
    return sum;
}
