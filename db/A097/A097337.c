/*
 * A097337  Integer part of the edge of a cube that has space-diagonal n.
 * https://oeis.org/A097337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊ n / √3 ⌋ = ⌊ √(⌊ n² / 3 ⌋) ⌋
 * Range:     u8 n <= 13, u16 n <= 306, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;            /* offset 1 – unused but safe */
    seq_t ns = (seq_t)n;                           /* convert index to seq_t */
    seq_t sq = ns * ns;                            /* n² */
    seq_t div = sq / (seq_t)3;                     /* ⌊n²/3⌋ */
    return floor_sqrt(div);                        /* ⌊√(⌊n²/3⌋)⌋ = ⌊n/√3⌋ */
}
