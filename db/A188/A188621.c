/*
 * A188621  Smallest number k>1 such that k*(n-th triangular number) is also a
 *          triangular number.
 * https://oeis.org/A188621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k>1 such that k*n*(n+1)/2 is triangular
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 350, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 80 terms through n = 902 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;                    /* undefined for n<1 */

    seq_t N = n;
    seq_t M = N * (N + 1);                  /* n*(n+1) */

    for (idx_t k = 2; ; ++k) {
        /* D = 1 + 4*k*M  (use shift to avoid overflow on 64‑bit builds) */
        seq_t prod = (seq_t)k * M;
        seq_t D = 1 + (prod << 2);
        seq_t s = floor_sqrt(D);
        if (s * s == D)                    /* perfect square found */
            return (seq_t)k;
    }
}
