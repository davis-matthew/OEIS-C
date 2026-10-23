/*
 * A126125  Triangle equal to the matrix square of the triangle
 *          binomial(n,floor((n+1-(-1)^(n+k)*(k+1))/2)).
 * https://oeis.org/A126125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, easy, nonn
 * Formula:   For the flattened triangle a(n) where the triangle T is obtained
 *            * by squaring the triangle B(n,k) defined by * * B(n,k) =
 *            binomial(n, * floor((n+1-(-1)^(n+k)*(k+1))/2)) * * the entry
 *            a(n) corresponds to the point (r,c) in the triangle with * r =
 *            floor((sqrt(8*n+1)-1)/2) * c = n - r*(r+1)/2 * * T...
 * Range:     u8 n <= 27, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"

/* Computes B(n,k) = binomial(n, floor((n+1-(-1)^(n+k)*(k+1))/2)) */
static seq_t B(idx_t n, idx_t k)
{
    /* (-1)^(n+k) is 1 when (n+k) is even, -1 when odd */
    if (((n + k) & 1ULL) == 0)          /* (n+k) even */
    {
        /* floor((n - k)/2)  == (n-k)/2 since the numerator is even */
        return binomial(n, (n - k) / 2);
    }
    else
    {
        /* floor((n + k + 2)/2) == (n + k + 2)/2 */
        return binomial(n, (n + k + 2) / 2);
    }
}

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index within the row */
    idx_t start = r * (r + 1) / 2;
    idx_t c = n - start;

    seq_t sum = 0;
    for (idx_t i = c; i <= r; ++i)
    {
        seq_t bi  = B(r, i);   /* B(r,i)  */
        seq_t bic = B(i, c);   /* B(i,c)  */
        sum += bi * bic;       /* accumulate product */
    }
    return sum;
}
