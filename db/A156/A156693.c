/*
 * A156693  Triangle T(n, k) = Product_{j=1..k} Product_{i=0..j-1} ( 1 -
 *          (n-k+1)*(i+1) ) with T(n, 0) = 1 and T(n, n) = n!, read by rows.
 * https://oeis.org/A156693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is obtained by reading the triangle T(n,k) by rows. T(r,c)
 *            = 1 if c == 0 = r! if c == r = ∏_{j=1..c} ∏_{i=0..j-1} (1 - (r -
 *            c + 1)*(i + 1)) otherwise. The row index r is the smallest
 *            integer such that (r+1)(r+2)/2 > n, and the column index c is n
 *            - r(r+1)/2
 * Range:     i8 n <= 17, i16 n <= 24, i32 n <= 32, i64 n <= 51, i128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* find row r so that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ( ((unsigned __int128)(r + 1) * (r + 2)) / 2 <= n )
        ++r;

    /* column index */
    idx_t prev = (unsigned __int128)r * (r + 1) / 2;
    idx_t c = n - prev;

    /* diagonal case: return factorial */
    if (c == r)
    {
        seq_t fact = 1;
        for (idx_t i = 1; i <= r; ++i)
            fact *= (seq_t)i;
        return fact;
    }

    seq_t val = 1;
    idx_t base = (r - c) + 1;

    for (idx_t j = 1; j <= c; ++j)
    {
        for (idx_t i = 0; i < j; ++i)
        {
            seq_t term = 1 - (seq_t)base * (i + 1);
            val *= term;
        }
    }

    return val;
}
