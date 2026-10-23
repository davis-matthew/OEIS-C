/*
 * A127733  Triangle read by rows: n-th row is n^2 preceded by (n-1) zeros.
 * https://oeis.org/A127733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ((sqrt(1+8n)-1)/2)^2 if 1+8n is a perfect square, 0
 *            otherwise
 * Range:     u8 n <= 7, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* Return the n-th term of OEIS A127733 (triangle read by rows: zeros then n^2). */
seq_t a(idx_t n)
{
    /* Compute d = 1 + 8*n in the type seq_t to avoid overflow of idx_t range. */
    seq_t d = (seq_t)1 + (seq_t)8 * (seq_t)n;

    /* s = floor(sqrt(d)) */
    seq_t s = floor_sqrt(d);

    /* If d is not a perfect square, the sequence is 0 at this index. */
    if (s * s != d)
        return (seq_t)0;

    /* Compute k = (s - 1) / 2, which is an integer because s is odd. */
    seq_t k = (s - (seq_t)1) / (seq_t)2;

    /* Return k^2 (n^2 for the last element of the row). */
    return k * k;
}
