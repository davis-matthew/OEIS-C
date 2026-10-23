/*
 * A183769  1/32 the number of (n+1) X 7 binary arrays with equal numbers of 2
 *          X 2 subblocks with sum mod two being 0 and 1.
 * https://oeis.org/A183769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^{n+2} * binom(6*n, 3*n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute the binomial coefficient C(6n, 3n) */
    seq_t val = binom(6 * n, 3 * n);
    /* multiply by 2^{n+2} */
    val *= pow2(n + 2);
    return val;
}
