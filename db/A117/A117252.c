/*
 * A117252  Triangle T, read by rows, where matrix power T^3 has powers of 3
 *          in the secondary diagonal: [T^3](n+1,n) = 3^(n+1), with all 1's in
 *          the main diagonal and zeros elsewhere.
 * https://oeis.org/A117252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,k) where r = largest m with m*(m+1)/2 ≤ n, k = n -
 *            r*(r+1)/2. For r > k ≥ 0: T(r,k) = (∏_{j=0}^{r-k-1}(1-3*j)) /
 *            (r-k)! * 3^{ r(r-1)/2 - k(k-1)/2 } . For k = r: T(r,r) = 1. The
 *            sequence is the row‑wise flattening of this lower triangular
 *            matrix
 * Range:     i8 n <= 9, i16 n <= 14, i32 n <= 20, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t pow3(idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)3;
    while (exp > 0)
    {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while (r * (r + 1) / 2 <= n)
        r++;
    /* r now exceeds the correct value by one. */
    r--;

    /* Column index within the row. */
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    /* Diagonal element. */
    if (k == r)
        return (seq_t)1;

    /* Compute the product ∏_{j=0}^{m-1} (1-3*j) and factorial(m). */
    idx_t m = r - k;
    seq_t prod = (seq_t)1;
    for (idx_t j = 0; j < m; j++)
        prod *= (seq_t)((seq_t)1 - (seq_t)3 * (seq_t)j);

    seq_t fact = (seq_t)1;
    for (idx_t i = 1; i <= m; i++)
        fact *= (seq_t)i;

    /* Exponent for 3. */
    idx_t e = r * (r - 1) / 2 - k * (k - 1) / 2;
    seq_t pow3e = pow3(e);

    /* Final value: (prod * pow3e) / fact.  Division is exact. */
    return (prod * pow3e) / fact;
}
