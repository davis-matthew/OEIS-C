/*
 * A047939  Number of 4-colorings of an n X n grid, up to rotational symmetry.
 * https://oeis.org/A047939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4^(n^2) + 2*4^((n^2 + 3*(n%2))/4) + 4^((n^2 + n%2)/2))/4
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;          /* convert index to seq_t */
    seq_t n2   = nseq * nseq;       /* n^2 */
    seq_t m    = (seq_t)4;          /* m = 4 */

    /* term1 = 4^(n^2) */
    seq_t term1 = big_pow(m, n2);

    /* term2 = 2 * 4^((n^2 + 3*(n%2))/4) */
    seq_t exp2  = (n2 + (seq_t)(3 * (n % 2))) / 4;
    seq_t term2 = (seq_t)2 * big_pow(m, exp2);

    /* term3 = 4^((n^2 + (n%2))/2) */
    seq_t exp3  = (n2 + (seq_t)(n % 2)) / 2;
    seq_t term3 = big_pow(m, exp3);

    /* a(n) = (term1 + term2 + term3) / 4 */
    seq_t sum = term1 + term2 + term3;
    return sum / 4;
}
