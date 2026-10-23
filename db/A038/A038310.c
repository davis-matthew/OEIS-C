/*
 * A038310  Triangle whose (i,j)-th entry is binomial(i,j)*10^(i-j)*8^j.
 * https://oeis.org/A038310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 10^(i-j) * 8^j * where i =
 *            floor((sqrt(8*n+1)-1)/2) and j = n - i*(i+1)/2
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binomial, floor_sqrt, pow10, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/floor_sqrt.h"
#include "../utils/pow_uint.h"
#include "../utils/pow10.h"

seq_t a(idx_t n) {
    /* compute row index i */
    seq_t temp  = (seq_t)n * (seq_t)8 + (seq_t)1;          /* 8*n + 1       */
    seq_t s     = floor_sqrt(temp);                       /* floor(sqrt(8*n+1)) */
    idx_t i     = (idx_t)((s - (seq_t)1) / (seq_t)2);     /* i = (sqrt-1)/2 */

    /* column index j within the row */
    idx_t row_sum = i * (i + (idx_t)1) / (idx_t)2;        /* sum of first i entries  */
    idx_t j       = n - row_sum;                          /* j = n - i*(i+1)/2      */

    /* compute the triangle entry */
    seq_t val = binomial(i, j);                           /* binomial(i,j)          */
    val = val * pow10(i - j);                             /* multiply by 10^(i-j)    */
    val = val * pow_uint(8, j);                           /* multiply by 8^j         */

    return val;
}
