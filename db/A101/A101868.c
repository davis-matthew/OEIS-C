/*
 * A101868  a(n) = n + 2*ceiling(phi*n), where phi = (1 + sqrt(5))/2. Row 1 of
 *          A101866.
 * https://oeis.org/A101868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + 2*ceil(phi*n), phi = (1 + sqrt(5))/2
 * Range:     u8 n <= 7, u16 n <= 1886, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses integer arithmetic only. It relies on the fact that */
/* ceil(phi*n) = floor(phi*n) + 1 where floor(phi*n) = (n + floor(sqrt(5)*n)) / 2. */
#include "../utils/isqrt.h"

/* Compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;              /* work in seq_t domain */
    seq_t t = 5 * nseq * nseq;          /* t = 5*n^2  */
    seq_t s = isqrt(t);                 /* s = floor(sqrt(5)*n) */
    seq_t floor_phi = (nseq + s) / 2;   /* floor(phi*n)  */
    seq_t a_val = nseq + 2 * (floor_phi + 1); /* a(n) = n + 2*ceil(phi*n) */
    return a_val;
}
