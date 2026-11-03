/*
 * A018852  a(n)^3 is smallest cube beginning with n.
 * https://oeis.org/A018852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) is the smallest integer k such that k³ starts with the
 *            decimal representation of n. For n = 0 the function returns 0.
 *            The implementation searches over the number of trailing digits t
 *            ≥ 0 and uses the cube‑root helper from the utils library
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 865, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9550 (2026-11-01)
 * Helpers:   ipow_i32, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/num_digits_idx.h"

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

/* multiply by 10^e   (e may be 0).  The result always fits in seq_t for
   the sizes used by this sequence. */
static inline seq_t pow10(idx_t e)
{
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i)
        r *= 10;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* t is the number of digits that follow the prefix n in the cube. */
    for (idx_t t = 0;; ++t)
    {
        seq_t ten   = pow10(t);
        seq_t low   = (seq_t)n * ten;                   /* n·10^t          */
        seq_t high  = ((seq_t)n + 1) * ten - 1;         /* (n+1)·10^t–1    */

        /* ceil cube root of low  →  floor_cube_root(low‑1)+1 */
        seq_t k     = floor_cube_root(low - 1) + 1;
        /* floor cube root of high */
        seq_t maxk  = floor_cube_root(high);

        if (k <= maxk)
            return k;                                   /* minimal k found  */
        /* otherwise increase t and try the next interval   */
    }
}
