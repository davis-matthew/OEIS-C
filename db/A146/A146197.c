/*
 * A146197  Number of n X n arrays of squares of integers summing to 6 with
 *          every element equal to at least one neighbor.
 * https://oeis.org/A146197
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n^6/3 - 4*n^5 + 14*n^4 - 178*n^3/3 - 886*n^2/3 +
 *            3958*n/3 - 614
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)18;
    if (n == 4) return (seq_t)1070;
    if (n < 2)  return (seq_t)0;  /* values below offset are irrelevant */

    /* For n >= 5 the sequence follows the empirical polynomial.
       Signed 128‑bit arithmetic is used to keep intermediate results
       exact, even for large n. */
    __int128 N   = (__int128)n;
    __int128 n2  = N * N;
    __int128 n3  = n2 * N;
    __int128 n4  = n3 * N;
    __int128 n5  = n4 * N;
    __int128 n6  = n5 * N;

    __int128 num = 4 * n6 - 12 * n5 + 42 * n4 - 178 * n3
                   - 886 * n2 + 3958 * N - 1842;

    return (seq_t)(num / 3);  /* the division is exact for all n >= 5 */
}
