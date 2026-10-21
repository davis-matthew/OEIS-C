/*
 * A080853  Square array of generalized polygonal numbers, read by
 *          antidiagonals.
 * https://oeis.org/A080853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = ((r^2)*c*(c-1)+2*r*c+2)/2 where (r,c) are the row and *
 *            column indices obtained by reading the square array T(r,c) by
 *            antidiagonals * in descending row order within each diagonal
 * Range:     u8 n <= 48, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   T_from_rc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/T_from_rc.h"

/* Compute the (row,column) pair for index n by antidiagonal enumeration. */
static void antidiagonal_coords(idx_t n, idx_t *row, idx_t *col)
{
    idx_t d = 0;
    /* Find the diagonal d such that n < (d+1)(d+2)/2. */
    for (;; ++d) {
        /* Using unsigned long long to avoid overflow. */
        unsigned long long terms = ((unsigned long long)(d + 1) * (d + 2)) / 2;
        if (n < terms)
            break;
    }
    unsigned long long prev = ((unsigned long long)d * (d + 1)) / 2;
    idx_t offset = n - prev;          /* position within diagonal (0 at top). */
    *row = d - offset;                /* descending row order. */
    *col = offset;
}

/* Requested sequence function. */
seq_t a(idx_t n)
{
    idx_t r, c;
    antidiagonal_coords(n, &r, &c);
    return T_from_rc(r, c);
}
