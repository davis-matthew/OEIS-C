/*
 * A190886  a(n) = [5nr]-5[nr], where r=sqrt(5).
 * https://oeis.org/A190886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(125*n^2)) - 5*floor(sqrt(5*n^2))
 * Range:     u8 none, u16 n <= 11, u32 n <= 132, u64 n <= 132, u128 n <= 132,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived from a(n) = [5 n sqrt(5)] - 5*[n sqrt(5)] using integer square roots. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute n^2 as seq_t to avoid overflow on small types */
    seq_t n_sq = (seq_t)n * (seq_t)n;

    /* floor(sqrt(5  n^2)) */
    seq_t floor5  = floor_sqrt((seq_t)5 * n_sq);

    /* floor(sqrt(125 n^2)) */
    seq_t floor125 = floor_sqrt((seq_t)125 * n_sq);

    /* a(n) = floor125 - 5*floor5 */
    return floor125 - 5 * floor5;
}
