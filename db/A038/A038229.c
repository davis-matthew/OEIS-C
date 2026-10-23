/*
 * A038229  Triangle whose (i,j)-th entry is binomial(i,j)*3^(i-j)*11^j.
 * https://oeis.org/A038229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   Let i be the unique integer with (i+1)(i+2)/2 > n. Let j = n -
 *            i*(i+1)/2. Then a(n) = binomial(i,j) * 3^(i-j) * 11^j
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial, pow3, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow3.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Find the row index i such that the cumulative
       number of terms up to that row exceeds n. */
    idx_t i = 0;
    while (((i + 1) * (i + 2)) / 2 <= n) {
        i++;
    }

    /* Starting index of row i in the flattened sequence. */
    idx_t row_start = i * (i + 1) / 2;
    idx_t j = n - row_start;          /* column index within the row */

    /* Compute the triangle entry: binomial(i,j) * 3^(i-j) * 11^j. */
    seq_t val = binomial(i, j);
    seq_t t3  = pow3(i - j);          /* 3^(i-j) */
    seq_t t11 = pow_uint(11, j);      /* 11^j */

    val = val * t3;
    val = val * t11;
    return val;
}
