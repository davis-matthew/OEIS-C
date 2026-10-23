/*
 * A022848  Integer nearest nx, where x = sqrt(5).
 * https://oeis.org/A022848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nearest integer to n*sqrt(5)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) = round(n*sqrt(5)) = round(sqrt(5*n^2)) */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* 1. Compute M = 5*n*n.  All operations are on seq_t. */
    seq_t N = (seq_t)n;
    seq_t M = 5 * N * N;

    /* 2. Compute the integer part of sqrt(M). */
    seq_t s = floor_sqrt(M);

    /* 3. Decide whether s or s+1 is the nearer integer. */
    seq_t s_sq  = s * s;
    seq_t diff1 = M - s_sq;        /* M - s^2  */
    seq_t sp1   = s + 1;
    seq_t diff2 = (sp1 * sp1) - M; /* (s+1)^2 - M */

    return diff1 < diff2 ? s : sp1;
}
