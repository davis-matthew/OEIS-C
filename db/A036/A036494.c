/*
 * A036494  Nearest integer to n^(9/2).
 * https://oeis.org/A036494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest integer to n^(9/2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 10, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* Utility for floor of integer square root of a seq_t value */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute n^9 */
    seq_t n128 = (seq_t)n;
    seq_t m = (seq_t)1;
    for (idx_t i = 0; i < 9; ++i)
        m *= n128;

    /* Floor of sqrt(m) gives the integer part of n^(9/2) */
    seq_t s  = floor_sqrt(m);
    seq_t s2 = s * s;
    seq_t s1 = s + 1;
    seq_t s12 = s1 * s1;

    /* Difference to nearest integer */
    seq_t diff1 = m - s2;  /* m - s^2      */
    seq_t diff2 = s12 - m; /* (s+1)^2 - m  */

    /* Choose the closer integer (ties round down) */
    return (diff1 <= diff2) ? s : s1;
}
