/*
 * A057364  a(n) = floor(8*n/21).
 * https://oeis.org/A057364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(8*n/21)
 * Range:     u8 n <= 671, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a 128‑bit intermediate to avoid overflow when multiplying
       large idx_t values by 8.  The final result will fit into seq_t
       for all tests and any overflow beyond seq_t’s width is truncated
       as required. */
    unsigned __int128 tmp = (unsigned __int128)n * 8 / 21;
    return (seq_t)tmp;
}
