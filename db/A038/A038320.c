/*
 * A038320  Triangle whose (i,j)-th entry is binomial(i,j)*11^(i-j)*6^j.
 * https://oeis.org/A038320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 11^(i-j) * 6^j, where (i,j) are the
 *            antidiagonal coordinates of n
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 45, u64 n <= 154, u128 n <= 538,
 *            big any n
 * Helpers:   big_pow, binomial, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* s = ⌊√(8·n + 1)⌋ as a seq_t value, then cast to idx_t */
    seq_t tmp   = 8 * (seq_t)n + 1;
    seq_t s_seq = floor_sqrt(tmp);
    idx_t s = (idx_t)s_seq;

    /* i = (s-1)/2   (row index) */
    idx_t i = (s - 1) / 2;

    /* j = n - i(i+1)/2   (column index within the row) */
    idx_t tri = i * (i + 1) / 2;
    idx_t j   = n - tri;

    /* evaluate the triangle entry */
    seq_t bin   = binomial(i, j);
    seq_t pow11 = big_pow((seq_t)11, (seq_t)(i - j));
    seq_t pow6  = big_pow((seq_t)6,  (seq_t)j);

    return bin * pow11 * pow6;
}
