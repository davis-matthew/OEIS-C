/*
 * A183197  Least integer k such that Floor(k*(n+1)^(1/3))>k*n^(1/3).
 * https://oeis.org/A183197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min k ≥ 1 such that floor(k*(n+1)^(1/3)) > k*n^(1/3)
 *            Implementation: inequality is equivalent to
 *            floor((k³*(n+1))^(1/3)) > floor((k³*n)^(1/3)). The comparison is
 *            done with 64‑bit integer arithmetic and a simple binary search
 *            for the integer cube root
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_cube_root_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_cube_root_ull.h"

seq_t a(idx_t n)
{
    idx_t k = 1;
    const unsigned long long un = (unsigned long long)n;
    for (;;) {
        unsigned long long kk = (unsigned long long)k;
        unsigned long long prod1 = kk * kk * kk * (un + 1ULL);
        unsigned long long prod2 = kk * kk * kk * un;
        seq_t r1 = floor_cube_root_ull(prod1);
        seq_t r2 = floor_cube_root_ull(prod2);
        if (r1 > r2) return (seq_t)k;
        ++k;
    }
}
