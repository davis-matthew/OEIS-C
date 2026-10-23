/*
 * A032123  Number of 2n-bead black-white reversible strings with n black
 *          beads.
 * https://oeis.org/A032123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For odd n = 2k+1: a(n) = binomial(4k+1, 2k) For even n
 *            = 2k (n > 0): a(n) = binomial(4k-1, 2k-1) + binomial(2k-1, k-1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 65,
 *            big any n
 * Helpers:   binom_generic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binom_generic.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* n is odd: n = 2k+1 */
    if (n & 1U) {
        idx_t k = (n - 1) >> 1;                    /* k = (n-1)/2   */
        return binom_generic(4 * k + 1U, 2 * k);
    }

    /* n is even: n = 2k,    k >= 1 */
    idx_t k = n >> 1;                              /* k = n/2       */
    seq_t part1 = binom_generic(4 * k - 1U, 2 * k - 1U);
    seq_t part2 = binom_generic(2 * k - 1U, k - 1U);
    return part1 + part2;
}
