/*
 * A175364  A175363(n^2).
 * https://oeis.org/A175364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer pairs (x,y) with |x|^3 + |y|^3 <= n²,
 *            where -n <= x,y <= n. The function iterates over |x| and counts
 *            admissible |y| using an integer cube root
 * Range:     u8 n <= 7, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    seq_t result = 0;

    /* n² fits comfortably in 64‑bit for all n up to 10 000. */
    idx_t n2_ull = n * n;
    seq_t n2 = (seq_t)n2_ull;

    for (idx_t absx = 0; absx <= n; ++absx) {
        idx_t x3_ull = absx * absx * absx;
        if (x3_ull > n2_ull) break;         /* no further |y| possible */

        seq_t x3 = (seq_t)x3_ull;
        seq_t t    = n2 - x3;                /* remaining budget for |y|³ */

        seq_t ymax = floor_cube_root(t);     /* floor( t^(1/3) ) */
        seq_t cnt  = (ymax << 1) + 1;        /* 2*ymax + 1  choices for y */

        if (absx == 0)
            result += cnt;                   /* x = 0 contributes once   */
        else
            result += (cnt << 1);            /* x = ±absx contributes twice */
    }

    return result;
}
