/*
 * A206224  Floor(k^2/4) appears 1+floor(k/2) times.
 * https://oeis.org/A206224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((1/4)*(floor(sqrt(4*n+1))-1)^2)
 * Range:     u8 n <= 15, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute 4*n + 1 as seq_t to avoid overflow on raw types */
    seq_t t = (seq_t)4 * (seq_t)n + 1;
    /* Floor integer square root of t */
    seq_t k = floor_sqrt(t);
    /* k >= 1 for all n >= 0, so safe to subtract 1 */
    seq_t m = k - 1;
    /* Return floor((m*m)/4) */
    return (m * m) / 4;
}
