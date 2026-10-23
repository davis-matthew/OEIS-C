/*
 * A022805  a(n) = B(n) + C(n) where B(n) is Beatty sequence [ n*sqrt(3) ] and
 *          C is complement of B.
 * https://oeis.org/A022805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*sqrt(3)) + floor((3*n + floor(n*sqrt(3)))/2)
 * Range:     u8 n <= 4, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute B(n) = floor(n*sqrt(3)) as floor_sqrt(3*n*n)           */
    seq_t nn = (seq_t)n * (seq_t)n;               /* n² */
    seq_t b   = floor_sqrt((seq_t)3 * nn);        /* B(n) */

    /* Compute C(n) = floor((3*n + b)/2)                            */
    seq_t c   = ((seq_t)3 * (seq_t)n + b) / 2;    /* C(n) */

    return b + c;                                /* a(n) = B(n)+C(n) */
}
