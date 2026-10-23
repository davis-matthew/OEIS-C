/*
 * A194121  Triangular array: T(n,k)=C(n+2,k)+C(n+2,k+1)+C(n+2,k+2), 0<=k<=n.
 * https://oeis.org/A194121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = C(r+2, k)+C(r+2, k+1)+C(r+2, k+2) where r =
 *            floor((sqrt(8n+1)-1)/2) is the row number and k = n - r(r+1)/2
 *            is the column within that row
 * Range:     u8 n <= 22, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   binom, offset_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binom.h"
#include "../utils/offset_row.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = (idx_t)((sqrtl((long double)8.0L * (long double)n + 1.0L) - 1.0L) / 2.0L);

    /* Correct possible rounding errors of the sqrt calculation */
    while ((r + 1) * (r + 2) / 2 <= n) r++;
    while (r * (r + 1) / 2 > n) r--;

    idx_t k = n - offset_row(r);

    /* Compute the triple sum of binomial coefficients */
    seq_t val = binom(r + 2, k);
    val += binom(r + 2, k + 1);
    val += binom(r + 2, k + 2);

    return val;
}
