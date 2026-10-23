/*
 * A038305  Triangle whose (i,j)-th entry is binomial(i,j)*10^(i-j)*3^j.
 * https://oeis.org/A038305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j)*10^(i-j)*3^j where i =
 *            floor((sqrt(8*n+1)-1)/2) and j = n - i*(i+1)/2
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 54, u64 n <= 173, u128 n <= 669,
 *            big any n
 * Helpers:   binom_, pow10_, pow3_
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_.h"
#include "../utils/pow10_.h"
#include "../utils/pow3_.h"

/*----------------------------------------------------------------------------*/
/* Compute binomial coefficient C(n,k) – multiplicative, avoiding large     */
/* factorials.  k is reduced to min(k, n-k) to keep intermediate values      */

/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Locate the triangular index (row i, column j) corresponding to n. */
    idx_t i = 0;
    while (((i + 1) * (i + 2)) / 2 <= n)
        ++i;

    idx_t j = n - (i * (i + 1)) / 2;

    /* Evaluate the triangle entry. */
    seq_t term = binom_(i, j);
    term = term * pow10_(i - j);
    term = term * pow3_(j);

    return term;
}
