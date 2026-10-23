/*
 * A151610  Number of permutations of 7 indistinguishable copies of 1..n
 *          arranged in a circle with exactly 2 adjacent element pairs in
 *          decreasing order.
 * https://oeis.org/A151610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*((7/2)*8^n - 49*n) for n > 1, a(1)=0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    seq_t ns = (seq_t)n;

    /* compute 8^n */
    seq_t pow8 = pow_uint(8ULL, n);

    /* (7/2)*8^n */
    seq_t t = 7 * pow8;
    t /= 2;

    /* 49*n */
    seq_t sub = 49 * ns;

    /* (7/2)*8^n - 49*n */
    seq_t val = t - sub;

    /* multiply by n */
    val *= ns;
    return val;
}
