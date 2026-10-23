/*
 * A027849  a(n) = (n+1)*(5*n^2+4*n+1).
 * https://oeis.org/A027849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(5*n^2+4*n+1)
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t term = (5 * N * N + 4 * N + 1) * (N + 1);
    return term;
}
