/*
 * A127648  Triangle read by rows: row n consists of n zeros followed by n+1.
 * https://oeis.org/A127648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = ((s-1)/2) if (n+1)+8*(n+1) is a perfect square s² with s
 *            odd, else 0. In other words, a(n) is the row number + 1 if n+1
 *            is a triangular number; otherwise 0
 * Range:     u8 n <= 6, u16 n <= 637, u32 n <= 9476, u64 n <= 9476,
 *            u128 n <= 9476, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t x); */

seq_t a(idx_t n)
{
    /* t = n + 1 (the index of the last element of a row in the triangle) */
    seq_t t = (seq_t)n + 1;

    /* D = 1 + 8 * t; must be computed in seq_t to avoid overflow. */
    seq_t D = (seq_t)1 + (seq_t)8 * t;

    /* s = floor(sqrt(D)) */
    seq_t s = floor_sqrt(D);

    /* Check whether D is a perfect square and whether s is odd. */
    if (s * s == D && (s & (seq_t)1))
    {
        /* a(n) = (s - 1) / 2 */
        return (s - 1) / 2;
    }
    else
    {
        return (seq_t)0;
    }
}
