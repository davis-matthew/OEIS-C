/*
 * A185721  Arises in the maximum number of C5's in a triangle-free graph.
 * https://oeis.org/A185721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊(n/5)^5⌋ = n^5 / 3125
 * Range:     u8 n <= 15, u16 n <= 45, u32 n <= 422, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t in the case of 64‑bit builds */

seq_t a(idx_t n)
{
    /* Compute n^5 without overflow (using unsigned __int128 for 64‑bit builds) */
    unsigned __int128 prod = 1;
    for (int i = 0; i < 5; ++i)
        prod *= (unsigned __int128)n;

    /* 5^5 = 3125 */
    prod /= (unsigned __int128)3125;

    return (seq_t)prod;
}
