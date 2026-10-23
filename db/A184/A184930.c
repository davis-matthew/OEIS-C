/*
 * A184930  a(n) = n + [rn/t] + [sn/t] + [un/t], where []=floor and
 *          r=sin(Pi/2), s=sin(Pi/3), t=sin(Pi/4), u=sin(Pi/5).
 * https://oeis.org/A184930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n·√2⌋ + ⌊n·√(3/2)⌋ + ⌊n·√(5–√5)/2⌋
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 120, u64 n <= 120, u128 n <= 120,
 *            big any n
 * Helpers:   floor_ratio, floor_sqrt, ratio_leq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The third term uses integer arithmetic; the fourth term is computed
   by binary search with an inequality that avoids negative
   intermediate values. */

#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) */
#include "../utils/floor_ratio.h"
#include "../utils/ratio_leq.h"

seq_t a(idx_t n)
{
    /* First term: n itself */
    seq_t term1 = (seq_t)n;

    /* Second term: ⌊n·√2⌋ = ⌊√(2·n²)⌋ */
    seq_t n_sq = (seq_t)n * (seq_t)n;
    seq_t term2 = floor_sqrt((seq_t)2 * n_sq);

    /* Third term: ⌊n·√(3/2)⌋ = ⌊√(6·n²)⌋ / 2 */
    seq_t term3 = floor_sqrt((seq_t)6 * n_sq) >> 1;

    /* Fourth term: ⌊n·√(5–√5)/2⌋ */
    seq_t term4 = floor_ratio(n);

    return term1 + term2 + term3 + term4;
}
