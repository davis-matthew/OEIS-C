/*
 * A195175  a(n) = 8*n - floor(4*n*r), where r=(1+sqrt(5))/2 (the golden
 *          ratio.)
 * https://oeis.org/A195175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n - floor(4*n*r), r = (1+sqrt(5))/2 Equivalent rewrite:
 *            a(n) = 6*n - floor(2*n*sqrt(5))
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 7261, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* k = 2*n */
    seq_t k = (seq_t)n * 2;

    /* floor(k*sqrt(5)) = floor(sqrt(5*k*k)) */
    seq_t N = (seq_t)5 * k * k;
    seq_t min_root = floor_sqrt(N);

    /* a(n) = 6*n - floor_root */
    seq_t result = ((seq_t)6) * (seq_t)n - min_root;
    return result;
}
