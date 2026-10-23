/*
 * A156730  Triangle T(n, k) = Product_{j=1..k} Product_{i=0..j-1} ( 1 -
 *          (n-k+1)*(3*i-2) ) with T(n, 0) = 1 and T(n, n) = n!, read by rows.
 * https://oeis.org/A156730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r, k) where r is the row number such that r*(r+1)/2 <=
 *            n < (r+1)*(r+2)/2 and k = n - r*(r+1)/2. T(r, k) is defined by:
 *            T(r,0) = 1; T(r,r) = r!; for 0 < k < r: T(r,k) = ∏_{j=1..k}
 *            ∏_{i=0..j-1} (1 - (r - k + 1) * (3*i - 2))
 * Range:     i8 n <= 12, i16 n <= 18, i32 n <= 25, i64 n <= 41, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t, etc. */

static seq_t factorial(idx_t n)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        res *= (seq_t)i;
    return res;
}

static seq_t t_value(idx_t row, idx_t col)
{
    if (col == (idx_t)0)
        return (seq_t)1;
    if (col == row)
        return factorial(row);

    const seq_t base = (seq_t)(row - col + 1);
    seq_t result = (seq_t)1;

    for (idx_t j = 1; j <= col; ++j)
    {
        seq_t sub = (seq_t)1;
        for (idx_t i = 0; i < j; ++i)
        {
            seq_t factor = (seq_t)1 -
                           base * (((seq_t)3 * (seq_t)i) - (seq_t)2);
            sub *= factor;
        }
        result *= sub;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* find the row r that contains index n */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* column within that row */
    idx_t k = n - r * (r + 1) / 2;

    return t_value(r, k);
}
