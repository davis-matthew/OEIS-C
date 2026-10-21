/*
 * A211905  Number of lower triangular n X n arrays colored with integers 0
 *          upwards introduced in row major order, with no element equal to
 *          any horizontal or vertical neighbor, and containing the value
 *          n(n+1)/2-2.
 * https://oeis.org/A211905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/8)*n^4 + (1/4)*n^3 - (9/8)*n^2 + (3/4)*n + 1 (for n >
 *            1); a(1) = 0
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first term explicitly. */
    if (n <= 1U) {
        return (seq_t)0;
    }

    /* Use 128‑bit arithmetic for the intermediate calculation.  */
    unsigned __int128 N = (unsigned __int128)n;

    /* Compute (n^4 + 2*n^3 - 9*n^2 + 6*n + 8) / 8.  */
    unsigned __int128 tmp = 
        N * N * N * N
      + (unsigned __int128)2 * N * N * N
      - (unsigned __int128)9 * N * N
      + (unsigned __int128)6 * N
      + (unsigned __int128)8;

    tmp /= 8U;

    return (seq_t)tmp;
}
