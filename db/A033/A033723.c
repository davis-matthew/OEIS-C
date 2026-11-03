/*
 * A033723  Product theta3(q^d); d | 11.
 * https://oeis.org/A033723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of integer solutions (k,l) ∈ ℤ² of k² + 11·l²
 *            = n
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9679 (2026-11-01)
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
    seq_t total = 0;
    for (idx_t L = 0;; ++L) {
        idx_t t = L * L;          /* L² (fits in idx_t for n ≤ 10 000)             */
        idx_t val = 11 * t;       /* 11·L²                                      */
        if (val > n) break;       /* no further solutions possible               */

        seq_t m = (seq_t)n - (seq_t)val;          /* remainder to be a square */
        seq_t k = floor_sqrt(m);                  /* integer sqrt of m       */

        if (k * k == m) {                         /* m is a perfect square   */
            seq_t kcnt = (k == 0) ? 1 : 2;        /* k solutions for given L   */
            total += (L == 0) ? kcnt : kcnt * 2; /* account for ±L           */
        }
    }
    return total;
}
