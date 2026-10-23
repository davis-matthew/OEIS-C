/*
 * A188590  [(n+1)*r] - [n*r], where r = 3/2 + sqrt(13)/2 and [...] denotes
 *          the floor function.
 * https://oeis.org/A188590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1)*r) - floor(n*r) where r = 3/2 + sqrt(13)/2.
 *            The computation uses integer arithmetic only: floor(n*r) = (3*n
 *            + floor(n*sqrt(13))) / 2. Since floor(n*sqrt(13)) =
 *            floor(sqrt(13*n^2)), we can evaluate it exactly with the
 *            provided floor_sqrt helper
 * Range:     u8 n <= 1, u16 n <= 34, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type to avoid overflow. */
    seq_t rn   = (seq_t)n;          /* n as seq_t */
    seq_t rn1  = rn + 1;            /* n+1 as seq_t */

    /* Compute floor(n*sqrt(13)) = floor(sqrt(13*n^2)) */
    seq_t n_sq = rn * rn;           /* n^2 */
    seq_t n1_sq = rn1 * rn1;        /* (n+1)^2 */
    seq_t t     = n_sq * ((seq_t)13);   /* 13*n^2 */
    seq_t t1    = n1_sq * ((seq_t)13);  /* 13*(n+1)^2 */

    seq_t s  = floor_sqrt(t);       /* floor(n*sqrt(13))  */
    seq_t s1 = floor_sqrt(t1);      /* floor((n+1)*sqrt(13)) */

    /* floor(n*r) = (3*n + floor(n*sqrt(13))) / 2  */
    seq_t floor_nr   = (rn * 3 + s) / 2;
    seq_t floor_nr1  = (rn1 * 3 + s1) / 2;

    /* a(n) = floor((n+1)*r) - floor(n*r) */
    return floor_nr1 - floor_nr;
}
