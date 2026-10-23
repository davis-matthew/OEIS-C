/*
 * A123203  a(n) = 2^(n+1) - 3*n.
 * https://oeis.org/A123203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) - 3*n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Compute 2^(n+1) */
    seq_t two_power = pow2(n + 1);
    /* Compute 3*n */
    seq_t three_times_n = (seq_t)n * 3;
    /* Return the difference */
    return two_power - three_times_n;
}
