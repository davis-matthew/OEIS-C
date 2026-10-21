/*
 * A075777  Minimal surface area of a rectangular solid with volume n and
 *          integer sides.
 * https://oeis.org/A075777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   minimal surface area of an integer‑sided rectangular solid with
 *            volume n a(n) = min{ 2(ab + ac + bc) | a,b,c ∈ ℕ, abc = n }
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>
#include "../utils/int_cube_root.h"

/* integer square root: largest k with k² ≤ x */
static idx_t int_sqrt(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        __int128 sq = (__int128)mid * mid;
        if (sq <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0) return 0;

    /* start with a very large value */
    seq_t best = UINT64_MAX;
    idx_t acube = int_cube_root(n);

    for (idx_t a = 1; a <= acube; ++a) {
        if (n % a != 0) continue;
        idx_t m = n / a;
        idx_t bsqrt = int_sqrt(m);

        for (idx_t b = a; b <= bsqrt; ++b) {
            if (m % b != 0) continue;
            idx_t c = m / b;
            seq_t surface = 2 *
                            ((seq_t)a * b + (seq_t)a * c + (seq_t)b * c);
            if (surface < best) best = surface;
        }
    }
    return best;
}
