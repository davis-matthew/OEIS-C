/*
 * A026351  a(n) = floor(n*phi) + 1, where phi = (1+sqrt(5))/2.
 * https://oeis.org/A026351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = floor(n*phi) + 1, phi = (1+sqrt(5))/2
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 5739, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The value floor(n*phi) can be computed by
    ⌊n*φ⌋ = ⌊(n + ⌊n*√5⌋) / 2⌋.
    The integer floor of n*√5 is obtained as ⌊√(5·n²)⌋.  */
#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    /* Compute floor(n * sqrt(5)) as floor(sqrt(5 * n^2)) */
    seq_t t = nseq * nseq;      /* n² */
    t *= (seq_t)5;              /* 5 * n² */
    seq_t s = isqrt_seq(t);     /* floor(sqrt(5 * n²)) = floor(n*sqrt(5)) */

    /* floor(n*phi) = (n + s) / 2 (integer division) */
    seq_t floor_nphi = (nseq + s) / 2;

    /* a(n) = floor(n*phi) + 1 */
    return floor_nphi + (seq_t)1;
}
