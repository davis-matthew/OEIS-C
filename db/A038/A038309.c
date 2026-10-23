/*
 * A038309  Triangle whose (i,j)-th entry is binomial(i,j)*10^(i-j)*7^j.
 * https://oeis.org/A038309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 10^(i-j) * 7^j, * where (i,j) satisfies n
 *            = i*(i+1)/2 + j, 0 <= j <= i
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Find the row index i such that triangular(i) <= n < triangular(i+1) */
    idx_t i = 0;
    idx_t t = 0;              /* t = i*(i+1)/2  */
    while (t + i + 1 <= n) {   /* triangular(i+1) = t + i + 1 */
        ++i;
        t += i;                /* now t = triangular(i) */
    }

    idx_t j = n - t;           /* column index, 0 <= j <= i */

    /* Compute the term: binomial(i,j) * 10^(i-j) * 7^j */
    seq_t coeff = binomial(i, j);
    seq_t p10 = pow_uint(10, i - j);
    seq_t p7  = pow_uint(7, j);

    seq_t res = coeff;
    res *= p10;
    res *= p7;
    return res;
}
