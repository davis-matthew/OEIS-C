/*
 * A213773  Rectangular array: (row n) = b**c, where b(h) = 3*h-2, c(h) =
 *          3*n-5+3*h, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 630, u32 n <= 1035, u64 n <= 1035,
 *            u128 n <= 1035, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A213773 – the array read by antidiagonals of the convolution of
 *  b(h)=3h-2 and c(h)=3n-5+3h.
 *
 *  For the entry at position (row, col) of the array,
 *      T(row,col) = Σ_{i=1}^{col}  (3*i-2) * (3*row-5+3*(col-i+1)).
 *
 *  The sequence a(n) is the antidiagonal traversal of that array,
 *  i.e. a(1)=T(1,1), a(2)=T(1,2), a(3)=T(2,1), …
 *
 *  This implementation finds the (row,col) belonging to index n
 *  by solving for the antidiagonal s such that
 *      s*(s+1)/2 ≥ n   and   (s-1)*s/2 < n.
 *  Then it evaluates the convolution explicitly.  The loops run
 *  at most O(√n) iterations, which is well below 2 s for n up to
 *  10 000.  All arithmetic is performed in seq_t; no negative
 *  intermediate values occur.
 */

#include <stdint.h>

seq_t a(idx_t N)
{
    /* Find the diagonal number s with s*(s+1)/2 >= N */
    idx_t lo = 1, hi = 1;
    while ((unsigned __int128)hi * (hi + 1) / 2 < N) hi <<= 1;

    while (lo < hi) {
        idx_t mid = (lo + hi) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= N)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t s = lo;

    /* Number of entries before this diagonal */
    idx_t prev = (unsigned __int128)(s - 1) * s / 2;
    /* 0‑based offset within the diagonal */
    idx_t offset = N - prev - 1;

    /* Map to (row, col) where row = offset+1, col = s-offset */
    idx_t row = offset + 1;
    idx_t col = s - offset;

    seq_t sum = 0;
    seq_t seq_row = (seq_t)row;

    /* Explicit convolution: Σ_{i=1}^{col} (3*i-2)*(3*row-5+3*(col-i+1)) */
    for (idx_t i = 1; i <= col; ++i) {
        seq_t bi = (seq_t)3 * (seq_t)i - 2;
        idx_t j = col - i + 1;
        seq_t cj = (seq_t)3 * seq_row - 5 + (seq_t)3 * (seq_t)j;
        sum += bi * cj;
    }

    return sum;
}
