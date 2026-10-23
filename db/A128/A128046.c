/*
 * A128046  Triangle read by rows: inverse of the lower triangular matrix
 *          (1/1; 1/1, 1/3; 1/1, 1/3, 1/5; ...).
 * https://oeis.org/A128046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows: inverse of the lower triangular matrix (1/1; 1/1, 1/3; 1/1, 1/3, 1/5; ...).
   a(n) is 0 for most indices.  For the r‑th row (r≥1) let
       j = n - r(r-1)/2   (1 ≤ j ≤ r).
   Then a(n) = -(2r-1) if j == r-1,
            a(n) =  (2r-1) if j == r,
            a(n) =   0   otherwise. */
#include <stdint.h>

/* Find the smallest r such that r*(r+1)/2 ≥ n. */
static inline idx_t triangular_row(idx_t n)
{
    idx_t lo = 1, hi = 1;
    /* grow hi until the triangular number reaches n */
    while ((__int128)hi * (hi + 1) / 2 < n)
        hi <<= 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) /* safety: OEIS offset is 1 */
        return 0;

    idx_t r = triangular_row(n);                      /* row containing n */
    idx_t prev_tri = (idx_t)((__int128)(r - 1) * r / 2); /* T_{r-1} */
    idx_t j = n - prev_tri;                              /* position in the row, 1‑based */

    seq_t odd = (seq_t)(2 * (seq_t)r - 1);                 /* 2r-1 fits in seq_t for the required sizes */

    if (j == r)
        return odd;                                    /* last entry of the row */
    if (j == r - 1)
        return -odd;                                   /* penultimate entry */

    return 0;                                           /* all earlier entries are zero */
}
