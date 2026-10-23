/*
 * A106602  Number of primitive positive solutions to 8n+2=x^2+y^2.
 * https://oeis.org/A106602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of primitive positive integer solutions (x, y) to
 *            8 n + 2 = x² + y², counted unordered (x ≤ y)
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd64.h"

/* Integer square root (unsigned 64‑bit).  Returns floor(sqrt(x)). */
static uint64_t isqrt64(uint64_t x)
{
    uint64_t r = 0;
    uint64_t bit = (uint64_t)1 << 62;               /* 2^62 <= 2^63-1 */
    while (bit > x) bit >>= 2;                     /* largest power of 4 ≤ x */
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Greatest common divisor for 64‑bit unsigned integers. */

seq_t a(idx_t n)
{
    /* Compute m = 8 n + 2 */
    seq_t m = (seq_t)n * 8 + 2;

    /* Maximum x with x <= y: x^2 <= m/2 */
    seq_t maxx = isqrt64((uint64_t)(m / 2));

    seq_t count = 0;
    for (seq_t x = 1; x <= maxx; ++x) {
        seq_t x2 = x * x;
        seq_t y2 = m - x2;
        seq_t y = isqrt64((uint64_t)y2);
        if (y * y != y2) continue;          /* y^2 not exact */
        if (y < x) continue;                /* enforce x <= y */
        if (gcd64((uint64_t)x, (uint64_t)y) == 1)
            ++count;
    }
    return count;
}
