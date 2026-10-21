/*
 * A218072  Product of the nonzero digits (in base 10) of n^2.
 * https://oeis.org/A218072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = product of the nonzero decimal digits of n^2
 * Range:     u8 n <= 42, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute n^2 using a 64‑bit type to avoid overflow for small
     * seq_t builds (e.g. uint8_t).  The resulting square is at most
     * 10000^2 = 1e8 for the expected test range, comfortably inside
     * 64 bits. */
    uint64_t sq = (uint64_t)n * (uint64_t)n;

    uint64_t prod = 1;
    while (sq > 0) {
        uint64_t d = sq % 10;
        if (d != 0)
            prod *= d;
        sq /= 10;
    }

    return (seq_t)prod;
}
