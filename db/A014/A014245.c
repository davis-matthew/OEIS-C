/*
 * A014245  a(n) = (n-th term of Beatty sequence for (3+sqrt(3))/2) - (n-th
 *          term of Beatty sequence for sqrt(3)).
 * https://oeis.org/A014245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((3+sqrt(3))/2 * n) - floor(sqrt(3) * n)
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed via integer arithmetic:
   floor(alpha*n) = floor((3*n + floor(n*sqrt(3))) / 2) */
/* Since floor(n*sqrt(3)) = floor(sqrt(3*n*n)) */

#include "../utils/floor_sqrt.h"   /* Provides floor_sqrt(seq_t) */

seq_t a(idx_t n)
{
    /* Convert index to seq_t */
    seq_t ns = (seq_t)n;                     /* n as seq_t */

    /* n^2 as seq_t */
    seq_t n_sq = ns * ns;

    /* floor(n*sqrt(3)) = floor(sqrt(3*n^2)) */
    seq_t thr_n_sq = (seq_t)3 * n_sq;         /* 3 * n^2 */
    seq_t floor_n_sqrt3 = floor_sqrt(thr_n_sq);

    /* floor((3*n + floor(n*sqrt3)) / 2) */
    seq_t numerator = (seq_t)3 * ns + floor_n_sqrt3;
    seq_t floor_alpha_n = numerator / (seq_t)2;

    /* result */
    return floor_alpha_n - floor_n_sqrt3;
}
