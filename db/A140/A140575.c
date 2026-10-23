/*
 * A140575  Triangle read by rows: the coefficient of [x^k] of the polynomial
 *          1-(x-1)^n in row n and column k, 0<=k<n.
 * https://oeis.org/A140575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, easy, sign
 * Formula:   a(n) is the n‑th element of the triangle T(n,k) * read by rows,
 *            where T(n,k) is the coefficient of x^k in * 1-(x-1)^n. Thus *
 *            T(n,0) = 1-(-1)^n, * T(n,k) = (-1)^{n-k+1} * C(n,k) (k>0)
 * Range:     i8 n <= 38, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - i + 1);
        res = res / (seq_t)i;
    }
    return res;
}

static seq_t triangle_value(idx_t r, idx_t k)
{
    if (k == 0)        /* special first column */
        return (r % 2 == 0) ? (seq_t)0 : (seq_t)2;

    seq_t bin = binom_idx(r, k);
    /* sign = (-1)^{r-k+1} */
    return ((r - k + 1) & 1) ? -bin : bin;
}

seq_t a(idx_t n)
{
    /* locate the row r that contains position n */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) ++r;

    /* index within that row */
    idx_t before = r * (r + 1) / 2;
    idx_t k = n - before;

    return triangle_value(r, k);
}
