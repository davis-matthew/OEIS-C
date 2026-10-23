/*
 * A184927  n+[rn/u]+[sn/u]+[tn/u], where []=floor and r=1, s=sqrt(3),
 *          t=sqrt(5), u=sqrt(7).
 * https://oeis.org/A184927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n/√7⌋ + ⌊n√3/√7⌋ + ⌊n√5/√7⌋ Computed without
 *            floating‑point by using integer arithmetic: a(n) = n +
 *            ⌊√(⌊n²/7⌋)⌋ + ⌊√(⌊3 n²/7⌋)⌋ + ⌊√(⌊5 n²/7⌋)⌋ (⌊x⌋ denotes the
 *            integer floor.)
 * Range:     u8 n <= 9, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n² as seq_t */
    seq_t sq = (seq_t)n * (seq_t)n;

    /* floor(√(⌊n²/7⌋)) */
    seq_t t1 = floor_sqrt(sq / 7);

    /* floor(√(⌊3 n²/7⌋)) */
    seq_t t2 = floor_sqrt((sq * 3) / 7);

    /* floor(√(⌊5 n²/7⌋)) */
    seq_t t3 = floor_sqrt((sq * 5) / 7);

    return (seq_t)n + t1 + t2 + t3;
}
