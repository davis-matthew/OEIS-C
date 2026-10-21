/*
 * A055248  Triangle of partial row sums of triangle A007318(n,m) (Pascal's
 *          triangle). Triangle A008949 read backwards. Riordan (1/(1-2x),
 *          x/(1-x)).
 * https://oeis.org/A055248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,m) where * r = floor((sqrt(8*n + 1) - 1)/2) (row
 *            index) * m = n - r*(r+1)/2 (column index) * T(r,m) = Σ_{k=m}^{r}
 *            binomial(r,k)
 * Range:     u8 n <= 35, u16 n <= 135, u32 n <= 527, u64 n <= 2079,
 *            u128 n <= 8000, big any n
 * Helpers:   binomial, row_for_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/row_for_idx.h"

seq_t
a(idx_t n)
{
    idx_t r   = row_for_idx(n);
    idx_t m   = n - r * (r + 1) / 2;

    /* Compute the partial sum Σ_{k=m}^r binomial(r,k) */
    seq_t sum = binomial(r, m);          /* first term */

    for (idx_t k = m + 1; k <= r; ++k)
    {
        sum += binomial(r, k);
    }
    return sum;
}
