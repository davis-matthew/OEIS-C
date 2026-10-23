/*
 * A146959  A functionally symmetric Polynomial as a triangle of coefficients:
 *          p(x,n)=If[n == 0, 1, (x + 1)^n + 2^(n - 3)*Sum[(3^(m-1) +
 *          2*m+(n-1) )*x^m*(1 + x^(n - 2*m)), {m, 1, n - 1}]].
 * https://oeis.org/A146959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th entry of the triangle of coefficients t(n,m) of
 *            the polynomial p(x,n) = If[n==0,1,(x+1)^n +
 *            2^(n-3)*Sum_{m=1}^{n-1}[(3^(m-1)+2*m+(n-1))*x^m*(1 +
 *            x^(n-2*m))]]. The sequence is obtained by listing the rows of
 *            t(n,m) (from n=0 onward) left to right
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root of a 64‑bit unsigned integer */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t low  = 0;
    uint64_t high = (uint64_t)1 << 32;   /* sqrt(2^64) = 2^32 */
    uint64_t ans  = 0;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

/* fast exponentiation for seq_t */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = (seq_t)1;
    while (exp) {
        if (exp & 1)
            res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    /* find row r so that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    __uint128_t tmp = (__uint128_t)8 * n + 1;
    uint64_t s = isqrt_u64((uint64_t)tmp);
    idx_t r = (idx_t)((s - 1) / 2);
    idx_t prevCnt = r * (r + 1) / 2;
    idx_t m = n - prevCnt;   /* column index within row r */

    /* small rows are trivial */
    if (r == 0) return (seq_t)1;
    if (r == 1) return (seq_t)1;
    if (r == 2) {
        if (m == 1) return (seq_t)6;
        return (seq_t)1;
    }

    /* binomial coefficient C(r, m) */
    idx_t k = m;
    if (k > r - k) k = r - k;   /* use symmetry */
    seq_t binom = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        binom = binom * (r + 1 - i) / i;
    }

    /* contributions from the sum part */
    seq_t termSum = (seq_t)0;

    /* first term (k = m) */
    if (m >= 1 && m <= r - 1) {
        seq_t pow3m = pow_seq((seq_t)3, m - 1);
        seq_t A = pow3m;
        A += (seq_t)(2 * m);
        A += (seq_t)(r - 1);
        termSum += A;
    }

    /* second term (k = r - m) */
    if (r - m >= 1 && r - m <= r - 1) {
        seq_t pow3k = pow_seq((seq_t)3, (r - m) - 1);
        seq_t A = pow3k;
        A += (seq_t)(2 * (r - m));
        A += (seq_t)(r - 1);
        termSum += A;
    }

    /* multiply by 2^(r-3) */
    seq_t factor = pow_seq((seq_t)2, r - 3);
    termSum *= factor;

    return binom + termSum;
}
