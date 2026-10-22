/*
 * A000328  Number of points of norm <= n^2 in square lattice.
 * https://oeis.org/A000328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 1 + 4*n + 4 * Sum_{x=1..n} floor(sqrt(n^2 - x^2)) (counts
 *            integer pairs (x,y) with x^2 + y^2 <= n^2)
 * Range:     u8 n <= 9, u16 n <= 144, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n * (uint64_t)n;
    uint64_t y = (uint64_t)n;
    seq_t s = 0;
    uint64_t x;
    for (x = 1; x <= (uint64_t)n; x++) {
        uint64_t r = nn - x * x;
        while (y * y > r) y--;
        s += (seq_t)y;
    }
    return (seq_t)1 + (seq_t)4 * (seq_t)n + (seq_t)4 * s;
}
