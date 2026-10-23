/*
 * A126026  Conjectured upper bound on area of the convex hull of any
 *          edge-to-edge connected system of regular unit hexagons
 *          (n-polyhexes).
 * https://oeis.org/A126026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ⌊(3*n*n + 14*n + 3)/18⌋
 * Range:     u8 n <= 36, u16 n <= 624, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* use a wider built‑in type for the intermediate calculation
       (n ≤ 10⁴ in typical tests, so 64‑bit is safe) */
    unsigned long long nn = (unsigned long long)n;
    unsigned long long val = 3ULL * nn * nn + 14ULL * nn + 3ULL;
    return (seq_t)(val / 18ULL);
}
