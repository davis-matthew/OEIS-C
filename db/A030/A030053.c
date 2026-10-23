/*
 * A030053  a(n) = binomial(2n+1,n-3).
 * https://oeis.org/A030053
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n+1, n-3) (offset 3)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Multiplicative binomial coefficient using gcd reductions to avoid overflow.
   Computes C(N,K) where 0 <= K <= N.  idx_t arguments are converted
   to seq_t for arithmetic. This implementation is safe for values that fit
   into seq_t and uses only integer operations. */
static seq_t binom_multiplicative(idx_t N, idx_t K)
{
    if (K > N - K)                /* use the smaller side of the symmetry */
        K = N - K;

    seq_t result = 1;

    for (idx_t i = 1; i <= K; ++i) {
        seq_t numerator   = (seq_t)(N - K + i);   /* (N-K+i)  */
        seq_t denominator = (seq_t)i;             /* i */

        /* Reduce with the current result first to keep numbers small */
        seq_t g = gcd(result, denominator);
        if (g > 1) {
            result   /= g;
            denominator /= g;
        }

        /* Reduce numerator-denominator pair */
        g = gcd(numerator, denominator);
        if (g > 1) {
            numerator   /= g;
            denominator /= g;
        }

        /* Final multiplication and division */
        result = result * numerator / denominator;
    }

    return result;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                   /* undefined for indices below the offset */

    return binom_multiplicative((2 * n + 1), (n - 3));
}
