/*
 * A186151  Rank of n^2 when {(1/4)i^3: i>=1} and {j^2>: j>=1} are jointly
 *          ranked with (1/4)i^3 after j^2 when (1/4)i^3=j^2. Complement of
 *          A186150.
 * https://oeis.org/A186151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊∛(4*n^2 – 1)⌋ (the rank of n² among the merged
 *            sequences of j² and i³/4, with j² preceding i³/4 when equal)
 * Range:     u8 n <= 4, u16 n <= 90, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for out‑of‑range indices */

    seq_t nn   = (seq_t)n;          /* cast index to seq_t first */
    seq_t sq   = nn * nn;          /* n² */
    seq_t v    = 4 * sq - (seq_t)1; /* 4*n² – 1 */
    seq_t c    = floor_cube_root(v);/* ⌊∛(4*n² – 1)⌋ */
    return nn + c;                 /* a(n) = n + c */
}
