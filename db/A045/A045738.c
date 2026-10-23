/*
 * A045738  Number of branches in all noncrossing rooted trees on n nodes on a
 *          circle.
 * https://oeis.org/A045738
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(3*n-3, n-2) - 2*binomial(3*n-6, n-3)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 26, u128 n <= 49,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Compute binomial coefficient C(N,K) for arbitrary N,K using
   multiplicative formula with GCD reductions to avoid overflow.  */
static seq_t _binom(idx_t N, idx_t K)
{
    if (K > N)               /* convention: C(N,K)=0 when K>N */
        return 0;
    if (K > N - K)           /* use symmetry */
        K = N - K;

    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        seq_t num = N - K + i;   /* term in the numerator */
        seq_t den = i;           /* term in the denominator */

        /* cancel common factors between current result and den */
        seq_t g = gcd(res, den);
        res /= g;
        den /= g;

        /* cancel common factors between num and remaining den */
        g = gcd(num, den);
        num /= g;
        den /= g;

        res *= num;
        res /= den;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;              /* undefined for indices below the offset */

    if (n == 2)                /* special case: second binomial has a negative lower index */
        return 1;              /* binomial(0,-1)=0 by convention */

    seq_t term1 = _binom(3 * n - 3, n - 2);
    seq_t term2 = _binom(3 * n - 6, n - 3);
    term2 += term2;            /* multiply by 2 */
    return term1 - term2;
}
