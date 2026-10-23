/*
 * A032085  Number of reversible strings with n beads of 2 colors. If more
 *          than 1 bead, not palindromic.
 * https://oeis.org/A032085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1) - 2^floor((n-1)/2), a(1) = 2
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)2;
    }
    idx_t exp_high = n - 1;
    idx_t exp_low  = exp_high >> 1;            /* floor((n-1)/2) */
    seq_t high = pow2(exp_high);
    seq_t low  = pow2(exp_low);
    return high - low;
}
