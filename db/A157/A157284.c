/*
 * A157284  Triangle T(n, k, m) = (m+1)^n*binomial(n,k)*f(n,m)*f(k,n-m)/n!,
 *          with T(n, 0, m) = 1, where f(n, k) = Product_{j=1..n} ( (1 -
 *          (k+1)^J)/(-k)^j ), f(n, 0) = n!, and m = 0, read by rows.
 * https://oeis.org/A157284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, less
 * Formula:   a(n) is the (row = r, col = k) entry of triangle T(r,k) = C(r,k)
 *            * Π_{j=1}^{k} ((r+1)^j – 1) / r^k, read row‐wise (r = 0,1,2,…)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 25, u64 n <= 34, u128 n <= 63,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2.
       (0‑based indexing of rows). */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    /* Column (k) within the row. */
    idx_t k = n - r * (r + 1) / 2;

    /* Base case: first element of every row is 1. */
    if (k == 0) return (seq_t)1;

    /* Start with the binomial coefficient C(r,k). */
    seq_t res = binom(r, k);

    /* Compute the product Π ((r+1)^j – 1) / r for j = 1…k. */
    seq_t base   = (seq_t)(r + 1);
    seq_t pow_val = 1;          /* holds (r+1)^(j-1) before each iteration */

    for (idx_t j = 1; j <= k; ++j) {
        pow_val = pow_val * base;                /* now (r+1)^j           */
        seq_t numerator = pow_val - 1;           /* ((r+1)^j – 1)         */
        res = res * numerator;                  /* Multiply numerator    */
        res = res / (seq_t)r;                   /* Divide by r (exact)   */
    }

    return res;
}
