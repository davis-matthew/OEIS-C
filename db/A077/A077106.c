/*
 * A077106  Largest integer cube <= n^2.
 * https://oeis.org/A077106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor_cuberoot((seq_t)n * (seq_t)n)^3 (largest cube <=
 *            n^2)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 6615, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_cuberoot, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_cuberoot.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t n_sq = (seq_t)n * (seq_t)n;     /* n^2 in seq_t */
    seq_t k     = floor_cuberoot(n_sq);   /* k = floor(cuberoot(n^2)) */
    return pow_seq(k, 3);                 /* k^3 is the desired cube */
}
