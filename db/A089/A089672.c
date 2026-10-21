/*
 * A089672  a(n) = S3(n,4), where S3(n, t) = Sum_{k=0..n} k^t *(Sum_{j=0..k}
 *          binomial(n,j))^3.
 * https://oeis.org/A089672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k^4 * ( Sum_{j=0..k} binomial(n,j) )^3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 15, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute binomial coefficients iteratively: C(n,0)=1, C(n,k)=
       C(n,k-1)*(n-k+1)/k.  Maintain running sum S(k) = Σ_{j=0}^{k} C(n,j). */
    seq_t result = 0;
    seq_t C = 1;          /* current binomial nCk   */
    seq_t S = 1;          /* current cumulative sum */

    for (idx_t k = 0; k <= n; ++k) {
        /* Update binomial and cumulative sum for k>0 */
        if (k > 0) {
            C = (C * (seq_t)(n - k + 1)) / k;
            S += C;
        }

        /* Compute k^4 (k=0 gives 0, so term will be 0) */
        seq_t kpow = (seq_t)k;          /* k (0 <= k <= n) */
        kpow = kpow * kpow;            /* k^2 */
        kpow = kpow * kpow;            /* k^4 */

        /* Compute S^3 = S * S * S */
        seq_t S2 = S * S;
        seq_t S3 = S2 * S;

        /* Add the term k^4 * S^3 to the result */
        result += S3 * kpow;
    }

    return result;
}
