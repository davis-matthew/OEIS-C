/*
 * A080746  Inverse Aronson transform of lower Wythoff sequence A000201.
 * https://oeis.org/A080746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth smallest integer belonging to the union of * S1(k) =
 *            ⌊φ·⌊φ·k⌋⌋ for k ≥ 1 * S2(k) = ⌊φ·⌊φ²·k⌋⌋ – 1 for k ≥ 2 * where φ
 *            = (1+√5)/2
 * Range:     u8 none, u16 n <= 54, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-01)
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

/* floor(n/φ) computed without floating point. */
static inline seq_t floor_div_phi(seq_t m)
{
    seq_t sq = floor_sqrt(m * m * 5);     /* floor(√(5·m²))  */
    return (sq - m) / 2;                  /* floor(m/φ)      */
}

/* floor(φ·m)     */
static inline seq_t floor_phi(seq_t m)
{
    return m + floor_div_phi(m);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* just to keep type safety */

    idx_t k1 = 1;                          /* index for S1   */
    idx_t k2 = 2;                          /* index for S2   */

    /* current values of the two sequences */
    seq_t val1 = floor_phi(floor_phi((seq_t)k1));            /* S1(k1)          */
    seq_t val2 = floor_phi(floor_phi((seq_t)k2) + (seq_t)k2) - 1;  /* S2(k2)          */

    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        if (val1 < val2)
        {
            result = val1;
            ++k1;
            val1 = floor_phi(floor_phi((seq_t)k1));
        }
        else if (val2 < val1)
        {
            result = val2;
            ++k2;
            val2 = floor_phi(floor_phi((seq_t)k2) + (seq_t)k2) - 1;
        }
        else /* equal values – advance both indices to avoid duplication */
        {
            result = val1;
            ++k1;
            ++k2;
            val1 = floor_phi(floor_phi((seq_t)k1));
            val2 = floor_phi(floor_phi((seq_t)k2) + (seq_t)k2) - 1;
        }
    }
    return result;
}
