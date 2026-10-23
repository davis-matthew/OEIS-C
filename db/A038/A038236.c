/*
 * A038236  Triangle whose (i,j)-th entry is binomial(i,j)*4^(i-j)*6^j.
 * https://oeis.org/A038236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j)*4^(i-j)*6^j where i =
 *            floor((sqrt(8*n+1)-1)/2) and j = n - i*(i+1)/2
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Find row i by solving i*(i+1)/2 <= n < (i+1)*(i+2)/2 */
    long double d = sqrtl(8.0L * (long double)n + 1.0L);
    idx_t i = (idx_t)((d - 1.0L) / 2.0L);

    /* Correct possible rounding errors */
    while ((i + 1) * (i + 2) / 2 <= n)
        ++i;
    while (i * (i + 1) / 2 > n)
        --i;

    idx_t j = n - i * (i + 1) / 2;

    seq_t bin = binomial(i, j);
    seq_t p4  = pow_uint(4, i - j);
    seq_t p6  = pow_uint(6, j);

    return bin * p4 * p6;
}
