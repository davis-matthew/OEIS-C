/*
 * A190236  a(n) = [n*u + n*v] -[n*u] -[n*v], where u=1/2, v=(1+sqrt(5))/2,
 *          and []=floor.
 * https://oeis.org/A190236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/2 + n*φ) - floor(n/2) - floor(n*φ) where φ =
 *            (1+√5)/2. The computation uses integer arithmetic only
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 5736, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    /* Compute m = floor(n*√5) by integer sqrt of 5*n^2. */
    seq_t n_sq   = (seq_t)n * n;          /* n² */
    seq_t five_n_sq = (seq_t)5 * n_sq;    /* 5⋅n² */
    seq_t m = isqrt_seq(five_n_sq);       /* floor(n*√5) */

    /* Ceil(n/2) = (n+1)/2 for integer arithmetic. */
    seq_t ceil_half = ((seq_t)n + 1) >> 1;

    /* floor(m/2) and floor((n+m)/2) . */
    seq_t m_half   = m >> 1;
    seq_t sum_half = (n + m) >> 1;

    /* Final formula: a(n) = ceil(n/2) + floor(m/2) - floor((n+m)/2). */
    return ceil_half + m_half - sum_half;
}
