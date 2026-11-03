/*
 * A232176  Least positive k such that n^2 + triangular(k) is a square.
 * https://oeis.org/A232176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive k such that n² + k(k+1)/2 is a square
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 82 terms through n = 4149 (2026-11-01)
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
    if (n == 0)
        return (seq_t)1;

    seq_t nsq = (seq_t)n * (seq_t)n;        /* n² */
    idx_t max_k = 4 * n - 2;                /* a(n) ≤ 4n − 2 */

    for (idx_t k = 1; k <= max_k; ++k) {
        seq_t tri = (seq_t)k * (seq_t)(k + 1) / 2;   /* k(k+1)/2 */
        seq_t s   = nsq + tri;                       /* target */
        seq_t m   = floor_sqrt(s);                   /* ⌊√s⌋ */
        if (m * m == s)                              /* exact square */
            return (seq_t)k;
    }
    /* Theoretically unreachable: lower bounds guarantee a solution. */
    return (seq_t)0;
}
