/*
 * A172277  floor(n*(sqrt(13)-sqrt(3))).
 * https://oeis.org/A172277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(sqrt(13)-sqrt(3)))
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 491, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the identity: floor(n*(sqrt(13)-sqrt(3))) =                */
/*   floor( sqrt(13*n^2) - sqrt(3*n^2) )                            */
/*   = floor(sqrt(13*n^2)) - floor(sqrt(3*n^2))  or  minus 1      */
/*   depending on whether the fractional parts satisfy               */
/*   frac(sqrt(13*n^2)) < frac(sqrt(3*n^2)).                        */

#include "../utils/floor_sqrt.h"   /* provides seq_t floor_sqrt(seq_t x) */

seq_t a(idx_t n)
{
    /* Compute m = n^2 as seq_t to avoid overflow of idx_t */
    seq_t m = (seq_t)n * n;

    /* Floor of sqrt(13*n^2) and sqrt(3*n^2) */
    seq_t s13 = floor_sqrt((seq_t)13 * m);
    seq_t s3  = floor_sqrt((seq_t)3  * m);

    /* Initial candidate difference */
    seq_t s  = s13 - s3;           /* s = floor(sqrt(13*n^2)) - floor(sqrt(3*n^2)) */

    /* Decision whether to subtract 1 or not:                */
    /* Test (10*m - s^2)^2 >= 12*m*s^2  (see proof in analysis). */
    seq_t L    = (seq_t)10 * m - s * s;
    seq_t left = L * L;
    seq_t right= (seq_t)12 * m * s * s;

    if (left >= right)
        return s;
    else
        return s - 1;
}
