/*
 * A109901  a(n) = binomial(n^2, n*(n+1)/2).
 * https://oeis.org/A109901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   gcd_any
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = binomial(n^2, n*(n+1)/2)
 *
 * Computes the binomial coefficient C(N,K) where
 *      N = n^2,
 *      K = n*(n+1)/2.
 * An efficient multiplicative algorithm is used with greatest‑common‑divisor
 * reductions to keep intermediate values within the limits of the destination
 * type.  The routine works for all supported seq_t types (unsigned integer
 * types up to 128 bits or an arbitrary‑precision integer class).
 */
#include "../utils/gcd_any.h"   /* provides gcd_any(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    /*  Compute N = n^2 and K = n*(n+1)/2.  */
    idx_t N = n * n;
    idx_t K = n * (n + 1) / 2;

    /*  Use symmetry: C(N,K) = C(N, N-K).  */
    if (K > N - K)
        K = N - K;

    /*  M = N - K will be used as the lower part of the numerator.  */
    idx_t M = N - K;

    seq_t res = 1;

    for (idx_t i = 1; i <= K; ++i) {
        seq_t num = (seq_t)(M + i);   /* numerator term   */
        seq_t den = (seq_t)i;         /* denominator term */

        /* cancel common factors between numerator and den */
        seq_t g1 = gcd_any(num, den);
        num /= g1;
        den /= g1;

        /* cancel remaining common factors between res and den */
        seq_t g2 = gcd_any(res, den);
        res /= g2;
        den /= g2;   /* den should now be 1 */

        /* multiply the reduced numerator into the result */
        res = res * num;
    }

    return res;
}
