/*
 * A014557  Multiplicity of K_3 in K_n.
 * https://oeis.org/A014557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = binomial(n,3) - floor( n/2 * floor( ((n-1)/2)^2 ) )
 * Range:     u8 n <= 9, u16 n <= 51, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;
    /* convert index to seq_t type once */
    seq_t nn = (seq_t)n;
    seq_t nm1 = nn - 1;

    /* t = floor( ((n-1)/2)^2 ) = floor( (n-1)^2 / 4 ) */
    seq_t sq = nm1 * nm1;
    seq_t t  = sq / 4;

    /* B = floor( n/2 * t ) = floor( n * t / 2 ) */
    seq_t b  = (nn * t) / 2;

    /* A = binomial(n,3) */
    seq_t a_val = binomial(n, 3);

    return a_val - b;
}
