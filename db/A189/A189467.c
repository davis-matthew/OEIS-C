/*
 * A189467  a(n) = n + [n*r/s] + [n*t/s]; r=1, s=1+sqrt(2), t=1/2+sqrt(2).
 * https://oeis.org/A189467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n(√2 − 1)⌋ + ⌊n(3 − √2)/2⌋
 * Range:     u8 n <= 5, u16 n <= 84, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 100 terms through n = 9238 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived using integer‑square‑root arithmetic */

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
    if (n == 0) return 0;   /* offset 1; n≥1 in practical use */

    /* m = ⌊n √2⌋  (use √(2·n²)) */
    seq_t m = floor_sqrt((seq_t)2 * (seq_t)n * (seq_t)n);

    /* even n: a(2k) = ⌊8k²⌋^½ - ⌊2k²⌋^½ + 3k - 1  */
    if ((n & 1) == 0) {               /* even */
        idx_t k = n >> 1;              /* k = n/2 */
        seq_t k_sq = (seq_t)k * (seq_t)k;
        seq_t m6 = floor_sqrt((seq_t)8 * k_sq);  /* ⌊√(8k²)⌋ = ⌊2k√2⌋ */
        seq_t m2 = floor_sqrt((seq_t)2 * k_sq);  /* ⌊√(2k²)⌋ = ⌊k√2⌋ */
        return m6 - m2 + (seq_t)3 * (seq_t)k - 1;
    }

    /* odd n: let  n = 2k+1  */
    /* intA = ⌊(3n−1)/2⌋  */
    seq_t intA = ((seq_t)3 * (seq_t)n - 1) / 2;
    /* floor_B = ⌊m/2⌋ = ⌊⌊n√2⌋/2⌋ = ⌊n√2/2⌋  */
    seq_t floor_B = m / 2;
    /* condition: m == 2·floor_B + 1 */
    seq_t cond = (m == 2 * floor_B + 1) ? 1 : 0;
    return intA - floor_B + m - cond;
}
