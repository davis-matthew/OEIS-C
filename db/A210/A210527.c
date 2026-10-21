/*
 * A210527  a(n) = 9*n^2 + 39*n + 83.
 * https://oeis.org/A210527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n^2 + 39*n + 83
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t n_t = n;
    return 9 * n_t * n_t + 39 * n_t + 83;
}
