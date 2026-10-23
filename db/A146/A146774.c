/*
 * A146774  Triangle read by rows: expansion of p(x,n)=If[n == 0, 1, (x + 1)^n
 *          + 2^(n+1)*Sum[Binomial[n-m, m]*x^m*(1 + x^(n - 2*m)), {m, 1, n -
 *          1}]].
 * https://oeis.org/A146774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less
 * Formula:   a(n) is the coefficient a(r,k) in the polynomial p(x,r) =
 *            (x+1)^r + 2^(r+1)*Σ_{m=1}^{r-1} binom(r-m,m)*x^m*(1+x^(r-2m))
 *            where r = floor((√(8n+1)-1)/2) and k = n - r(r+1)/2. The
 *            coefficient a(r,k) = binom(r,k) + 2^(r+1)*[ binom(r-k,k) +
 *            (binom(k,r-k) if k≠r-k) ]
 * Range:     u8 n <= 15, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find row r and column k such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    idx_t offset = 0;
    while (offset + r + 1 <= n)
    {
        offset += r + 1;
        r++;
    }
    idx_t k = n - offset;              /* 0 ≤ k ≤ r */

    /* Base contribution from (x+1)^r */
    seq_t result = binomial(r, k);

    /* Sum term contributions when k is not an endpoint */
    if (k > 0 && k < r)
    {
        seq_t pow = pow2(r + 1);                 /* 2^(r+1) */
        seq_t term1 = binomial(r - k, k);       /* binom(r-k, k) */

        if (k == r - k)                         /* m == r/2 case: double the term */
        {
            result += pow * term1 * 2;
        }
        else                                    /* two distinct terms */
        {
            seq_t term2 = binomial(k, r - k);   /* binom(k, r-k) */
            result += pow * term1;
            result += pow * term2;
        }
    }

    return result;
}
