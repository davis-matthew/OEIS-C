/*
 * A140074  Excess over the asymptote of the number of perfect squares between
 *          cubes.
 * https://oeis.org/A140074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(sqrt((n+1)^3-1)) - ceil(sqrt(n^3)) + 1 - floor(1.5
 *            * sqrt(n))
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* Convert index to seq_t for arithmetic */
    seq_t N = (seq_t)n;

    /* Compute floor_sqrt((n+1)^3 - 1) */
    seq_t N1 = N + (seq_t)1;
    seq_t N1c = N1 * N1 * N1;       /* (n+1)^3 */
    seq_t s2 = floor_sqrt(N1c - (seq_t)1);  /* floor sqrt((n+1)^3-1) */

    /* Compute ceil(sqrt(n^3)) */
    seq_t N3 = N * N * N;           /* n^3 */
    seq_t s1_floor = floor_sqrt(N3);
    seq_t s1 = (s1_floor * s1_floor == N3) ? s1_floor : s1_floor + (seq_t)1;

    /* Number of squares between the cubes */
    seq_t count;
    if (s1 <= s2)
        count = s2 - s1 + (seq_t)1;
    else
        count = (seq_t)0;

    /* Approximate asymptote: floor(1.5 * sqrt(n)) = floor_sqrt(9*n) / 2 */
    seq_t asym = floor_sqrt(N * (seq_t)9) / (seq_t)2;

    /* Excess over the asymptote */
    return count - asym;
}
