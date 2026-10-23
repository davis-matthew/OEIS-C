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
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for out‑of‑range indices */

    seq_t nn   = (seq_t)n;          /* cast index to seq_t first */
    seq_t sq   = nn * nn;          /* n² */
    seq_t v    = 4 * sq - (seq_t)1; /* 4*n² – 1 */
    seq_t c    = floor_cube_root(v);/* ⌊∛(4*n² – 1)⌋ */
    return nn + c;                 /* a(n) = n + c */
}
