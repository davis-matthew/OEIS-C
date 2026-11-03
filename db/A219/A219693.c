/*
 * A219693  The number of symmetric positive definite 2 X 2 matrices whose
 *          entries are integers of absolute value at most n.
 * https://oeis.org/A219693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{a=1..n} Σ_{b=1..n} (2*min(n, floor_sqrt(a*b-1)) + 1)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    if (n < 1) return 0;   /* undefined for n < 1, safe return */

    seq_t total = 0;
    for (idx_t a = 1; a <= n; ++a) {
        for (idx_t b = 1; b <= n; ++b) {
            idx_t prod = a * b;          /* prod ≤ n² ≤ 1e8 for n ≤ 10000 */
            idx_t s    = prod - 1;       /* non‑negative */
            idx_t r    = (idx_t)floor_sqrt((seq_t)s);
            if (r > n) r = n;            /* safety, not needed for n ≤ 10000 */
            total += (seq_t)(2 * r + 1);
        }
    }
    return total;
}
