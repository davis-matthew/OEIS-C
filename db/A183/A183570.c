/*
 * A183570  a(n) = n + floor(sqrt(n + 1)).
 * https://oeis.org/A183570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(n + 1))
 * Range:     u8 n <= 62, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;          /* convert index using seq_t */
    seq_t s  = floor_sqrt(tn + 1);/* floor(sqrt(n + 1)) in seq_t */
    return tn + s;                /* a(n) = n + floor(sqrt(n + 1)) */
}
