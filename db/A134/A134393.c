/*
 * A134393  Row sums of triangle A134392.
 * https://oeis.org/A134393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n^2-3*n+8)/12
 * Range:     u8 n <= 7, u16 n <= 30, u32 n <= 476, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Perform the calculation in a wide unsigned integer type.
       This guarantees correct results for every n that fits into
       the target seq_t. */
    uint64_t N   = n;                                // promote to 64‑bit
    uint64_t num = N * (N + 1) * (N * N - 3 * N + 8); // numerator
    uint64_t res = num / 12;                          // divide by 12

    return (seq_t)res;                                // truncate to seq_t
}
