/*
 * A194698  a(n) = floor((n - 1)/12) - floor((n^2 - 1)/(24*n)).
 * https://oeis.org/A194698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n - 1)/12) - floor((n^2 - 1)/(24*n))
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1, but guard against n==0 */
    seq_t N = (seq_t)n;                  /* work in seq_t to avoid overflow issues */
    seq_t term1 = (N - 1) / 12;           /* floor((n - 1)/12) */
    seq_t term2 = (N * N - 1) / (24 * N); /* floor((n^2 - 1)/(24*n)) */
    return term1 - term2;
}
