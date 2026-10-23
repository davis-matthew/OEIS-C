/*
 * A147301  a(n) = smallest value of parameter m such that the function rad(m
 *          n (n - m)) has maximal value n=2,3,4..., 0 < m < n where the
 *          function rad(k) (also called radical(k)) is the product of
 *          distinct prime divisors of k.
 * https://oeis.org/A147301
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m, 0 < m < n, that maximizes rad(m*n*(n-m))
 * Range:     u8 n <= 10, u16 n <= 64, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A147301. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/rad.h"

/* idx_t and seq_t are defined by the build environment. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* offset is 2; values below are irrelevant */

    uint64_t best_rad = 0;        /* largest radical found so far */
    seq_t best_m = 1;             /* corresponding m */

    /* Scan only the first half; symmetry ensures the smallest m that
       achieves the maximum is in 1 .. floor(n/2).  For n = 2 this still
       works because floor(2/2) == 1. */
    idx_t limit = n / 2;
    for (idx_t m = 1; m <= limit; ++m) {
        /* Compute m * n * (n - m) safely in 64‑bit. */
        uint64_t product = (uint64_t)m * n * (n - m);

        /* rad(product) returns a seq_t, but the value is guaranteed to
           be ≤ product ≤ 10^12 when n ≤ 10^4, so it fits in uint64_t. */
        uint64_t rad_val = (uint64_t)rad(product);

        if (rad_val > best_rad) {
            best_rad = rad_val;
            best_m = m;
        }
    }

    return best_m;
}
