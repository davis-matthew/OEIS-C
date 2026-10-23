/*
 * A186386  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f(i)=5i and g(j)=j(j+1)/2 (triangular
 *          number). Complement of A186385.
 * https://oeis.org/A186386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 63, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Adjusted joint rank of multiples of 5 and triangular numbers.
   a(n) is the 1‑based rank of the n‑th triangular number
   T(n) = n(n+1)/2 in the sorted set {5i} ∪ {j(j+1)/2},
   where ties are broken by putting the triangular number before
   the multiple of 5 when they are equal.
*/

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* use 64‑bit intermediates to avoid overflow on small seq_t */
    uint64_t n64 = (uint64_t)n;
    uint64_t tn64 = n64 * (n64 + 1) / 2;           /* T(n) */
    uint64_t fcnt = tn64 / 5;                      /* # of multiples of 5 ≤ T(n) */
    uint64_t sq = (uint64_t)floor_sqrt((seq_t)(1 + 8 * tn64));
    uint64_t gcnt = (sq - 1) / 2;                  /* # of triangular ≤ T(n) */
    /* subtract 1 if T(n) itself is a multiple of 5 (it is then counted twice) */
    uint64_t adjust = (tn64 % 5 == 0) ? 1 : 0;
    return (seq_t)(fcnt + gcnt - adjust);
}
