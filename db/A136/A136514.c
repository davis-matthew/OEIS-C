/*
 * A136514  Number of unit square lattice cells inside half-plane (two
 *          adjacent quadrants) of origin centered circle of radius n.
 * https://oeis.org/A136514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2 * Σ_{k=1}^{n-1} ⌊√(n² – k²)⌋
 * Range:     u8 n <= 13, u16 n <= 204, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for unsigned 64‑bit values. */
static inline idx_t int_isqrt(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq = mid * mid;
        if (sq == n) {
            return mid;
        }
        if (sq < n) {
            low = mid + 1;
        } else {
            if (mid == 0) {
                break;
            }
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n <= 1) {
        return 0;
    }

    idx_t n2 = n * n;
    seq_t sum = 0;

    for (idx_t k = 1; k < n; ++k) {
        idx_t t = n2 - k * k;
        idx_t r = int_isqrt(t);
        sum += (seq_t)r;
    }

    return sum * 2;
}
