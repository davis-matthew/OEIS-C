/*
 * A200872  Number of 0..n arrays x(0..3) of 4 elements without any interior
 *          element greater than both neighbors or less than both neighbors.
 * https://oeis.org/A200872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 18 n^3 + 47 n^2 + 42 n)/12 + 1
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Perform calculations in 64‑bit unsigned to avoid overflow of the
       intermediate value when n is modest (e.g., up to 10 000). */
    uint64_t N   = (uint64_t)n;
    uint64_t num = N * N * N * N                 /* n^4   */
                 + 18ULL * N * N * N             /* 18 n^3*/
                 + 47ULL * N * N                 /* 47 n^2*/
                 + 42ULL * N;                    /* 42 n  */
    uint64_t res = num / 12ULL + 1ULL;           /* integer division, result is exact */
    return (seq_t)res;
}
