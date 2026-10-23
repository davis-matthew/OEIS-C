/*
 * A140934  Number of 2 X 11 matrices with elements in 0..n with each row and
 *          each column in nondecreasing order. 2,11,n can be permuted, see
 *          formula.
 * https://oeis.org/A140934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n+12)/(12*n+12))*C(n+11,11)^2 where C is binomial
 *            coefficient
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 6, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /*  C(n+11,11)  */
    idx_t k = 11;
    idx_t m = n + k;
    seq_t bin = binomial(m, k);

    /*  bin^2 * (n+12)  */
    seq_t num = bin * bin;
    num = num * ((seq_t)n + 12);

    /*  division by (12*n+12)  */
    seq_t den = (seq_t)(12*n + 12);
    return num / den;
}
