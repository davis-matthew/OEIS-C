/*
 * A056137  Number of ways in which n can be the longer leg (middle side) of
 *          an integer-sided right triangle.
 * https://oeis.org/A056137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer‑sided right triangles in which n is the
 *            longer leg, computed by brute‑forcing all possible shorter legs
 *            a (1 ≤ a < n) and checking whether a² + n² is a perfect square
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Integer square root for 64‑bit unsigned values; used in the brute force test. */
static uint64_t isqrt_uint64(uint64_t v)
{
    uint64_t lo = 0, hi = ((uint64_t)1 << 32);           /* hi^2 ≥ v for v < 2^64 */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    uint64_t n64 = (uint64_t)n;
    uint64_t n_sq = n64 * n64;

    uint64_t count = 0;
    for (idx_t a = 1; a < n; ++a) {
        uint64_t a64 = (uint64_t)a;
        uint64_t sum = a64 * a64 + n_sq;
        uint64_t c = isqrt_uint64(sum);
        if (c * c == sum)
            count++;
    }

    return (seq_t)count;
}
