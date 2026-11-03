/*
 * A177025  Number of ways to represent n as a polygonal number.
 * https://oeis.org/A177025
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ k ≥ 3 | ∃ m ≥ 1 : n = ((k-2)m² – (k-4)m)/2 } This
 *            function counts the number of polygonal families k that
 *            represent n. Each n ≥ 3 is always represented as the n-gonal
 *            number with m = 2, so the result is always at least 1. The
 *            algorithm iterates over the possible values of m, derived fro...
 * Range:     u8 n <= 7, u16 n <= 1308, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 114 terms through n = 8366 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
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

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                /* result for indices below the offset
                                     is irrelevant */

    seq_t count = 1;             /* m = 2 gives k = n, always valid */

    /* Upper bound for m:
       from 2*n ≥ m² + m  ⟺  m ≤ ( –1 + √(1 + 8n) ) / 2 */
    idx_t val = 1 + 8 * n;
    seq_t sqrt_val = floor_sqrt((seq_t)val);
    idx_t max_m = (idx_t)((sqrt_val > 0) ? ((sqrt_val - 1) / 2) : 0);

    if (max_m < 3)
        return count;            /* no further m values to test */

    for (idx_t m = 3; m <= max_m; ++m) {
        idx_t num = 2 * n + 2 * m * (m - 2);  /* numerator of k */
        idx_t den = m * (m - 1);              /* denominator of k */

        if (num % den == 0) {
            seq_t k = (seq_t)(num / den);
            if (k >= 3)
                ++count;
        }
    }

    return count;
}
