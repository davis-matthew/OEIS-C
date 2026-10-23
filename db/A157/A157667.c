/*
 * A157667  a(n) = 531441*n^2 - 740664*n + 258065.
 * https://oeis.org/A157667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 531441*n^2 - 740664*n + 258065
 * Range:     u8 none, u16 n <= 1, u32 n <= 90, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert index to seq_t once for reuse */
    seq_t N = n;

    /* Use 64‑bit arithmetic for intermediate values to avoid overflow
       of the 32‑bit target type when n is near its upper bound. */
    uint64_t tmp = (uint64_t)531441 * (uint64_t)(N * N);
    tmp -= (uint64_t)740664 * (uint64_t)N;
    tmp += 258065;

    return (seq_t)tmp;
}
