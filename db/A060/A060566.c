/*
 * A060566  a(n) = n^2 - 79*n + 1601.
 * https://oeis.org/A060566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 - 79*n + 1601
 * Range:     u8 none, u16 n <= 295, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    /* Compute n^2 + 1601 - 79*n to avoid unsigned underflow */
    return n_t * n_t + (seq_t)1601 - (seq_t)79 * n_t;
}
