/*
 * A180122  First of three "least, sum, least" self-generating sequences.
 * https://oeis.org/A180122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(1+sqrt(3))) - 1
 * Range:     u8 n <= 4, u16 n <= 62, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements sequence A180122 */
#include "../utils/floor_sqrt.h"

/* Returns the nth term of the sequence, with a(1) = 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n = 0, return 0 as a safe default */

    seq_t ns = (seq_t)n;
    /* Compute floor(n*sqrt(3)) as floor(sqrt(3*n*n)) */
    seq_t t = ns * ns * (seq_t)3;
    seq_t sq = floor_sqrt(t);

    /* a(n) = floor(n*sqrt(3)) + n - 1 */
    return sq + ns - (seq_t)1;
}
