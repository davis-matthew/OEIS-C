/*
 * A184008  n+floor(sqrt(7n/3)); complement of A184009.
 * https://oeis.org/A184008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(7n/3))
 * Range:     u8 n <= 27, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* Computes a(n) as defined for OEIS sequence A184008.
   Works with any seq_t and idx_t types provided by the build. */
seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;     /* cast index to seq_t for arithmetic */
    seq_t t = N * 7;        /* 7*n */
    seq_t div3 = t / 3;     /* integer division gives floor(7*n/3) */
    seq_t root = floor_sqrt(div3);  /* floor(sqrt(floor(7*n/3))) */
    return N + root;
}
