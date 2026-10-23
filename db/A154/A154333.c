/*
 * A154333  Difference between n^3 and the next smaller square.
 * https://oeis.org/A154333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^3 - floor(sqrt(n^3 - 1))^2
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the difference between n^3 and the next smaller perfect square. */

#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t x) is provided by the utils library */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type to avoid intermediate overflow */
    seq_t x = (seq_t) n;

    /* Compute n^3 */
    seq_t n3 = x * x * x;

    /* Compute floor(sqrt(n^3 - 1)) */
    seq_t sqroot = floor_sqrt(n3 - (seq_t)1);

    /* Square the root to get the largest square < n^3 */
    seq_t sq = sqroot * sqroot;

    /* The desired difference */
    return n3 - sq;
}
