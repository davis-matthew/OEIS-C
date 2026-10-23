/*
 * A158474  Triangle read by rows generated from (x-1)*(x-2)*(x-4)*...
 * https://oeis.org/A158474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = coefficient of x^(r-k) in ∏_{i=0}^{r-1}(x-2^i). Here r is
 *            the unique integer with r(r+1)/2 ≤ n < (r+1)(r+2)/2, and k = n -
 *            r(r+1)/2. Hence a(n) = T(r,k) where T(r,k) are the coefficients
 *            of (x-1)(x-2)(x-4)…(x-2^(r-1))
 * Range:     i8 n <= 16, i16 n <= 24, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define MAX_R 2048   /* sufficient for all tested n (n ≤ 10000 ⇒ r ≤ 141) */

seq_t a(idx_t n)
{
    /* find r and k */
    idx_t r = 0;
    idx_t cumulative = 0;
    while (n >= cumulative + (r + 1))
    {
        cumulative += (r + 1);
        ++r;
    }
    idx_t k = n - cumulative;

    /* compute coefficients for row r */
    seq_t coeff[MAX_R] = {0};
    coeff[0] = 1;
    seq_t p = 1; /* 2^0 */

    for (idx_t i = 1; i <= r; ++i)
    {
        /* multiply current polynomial by (x - p) */
        for (idx_t e = i; e > 0; --e)
        {
            coeff[e] = coeff[e - 1] - p * coeff[e];
        }
        coeff[0] = -p * coeff[0];
        p = p * 2;
    }

    return coeff[r - k];
}
