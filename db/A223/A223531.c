/*
 * A223531  Triangle S(n,k) by rows: coefficients of
 *          6^((n-1)/2)*(x^(1/6)*d/dx)^n when n=1,3,5,...
 * https://oeis.org/A223531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth term of the triangle S(r,k) by rows where r is
 *            the row number such that r(r+1)/2 >= n, and k = n - r(r-1)/2.
 *            The coefficients are given by S(r,k) = C(r-1,k-1) * 6^(k-1) *
 *            ∏_{i=k}^{r-1} (6i+1). This follows directly from the definition
 *            of the operator 6^((n-1)/2)*(x^(1/6)d/dx)^n wh...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 29, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   // seq_t binomial(idx_t n, idx_t k);
#include "../utils/pow_uint.h"   // seq_t pow_uint(uint64_t base, idx_t exp);

seq_t a(idx_t n)
{
    /* Find the row r and position k within the row */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;

    idx_t prev = r * (r - 1) / 2;   /* total terms before row r */
    idx_t k = n - prev;             /* 1‑based index within the row */

    /* Compute binomial coefficient C(r-1, k-1) */
    seq_t result = binomial(r - 1, k - 1);

    /* Multiply by 6^(k-1) */
    result *= pow_uint(6, k - 1);

    /* Multiply by the product of (6*i + 1) for i = k … r-1 */
    for (idx_t i = k; i <= r - 1; ++i)
    {
        result *= (seq_t)(6 * i + 1);
    }

    return result;
}
