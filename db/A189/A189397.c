/*
 * A189397  a(n) = n + [n*r/t] + [n*s/t]; r=1, s=1/sqrt(2), t=sqrt(3).
 * https://oeis.org/A189397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n/√3) + floor(n/√6)
 * Range:     u8 n <= 13, u16 n <= 104, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed using integer arithmetic only:
   floor(n/√3) = floor_sqrt( floor(n²/3) )
   floor(n/√6) = floor_sqrt( floor(n²/6) )
   where floor_sqrt(x) returns ⌊√x⌋ as seq_t.
*/
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert n to the seq_t type for arithmetic. */
    seq_t nn = (seq_t)n;
    /* Compute n² once. */
    seq_t sq = nn * nn;

    /* floor(n/√3) = floor_sqrt( floor(n²/3) ) */
    seq_t term1 = floor_sqrt( sq / 3 );

    /* floor(n/√6) = floor_sqrt( floor(n²/6) ) */
    seq_t term2 = floor_sqrt( sq / 6 );

    /* Final result. */
    return nn + term1 + term2;
}
