/*
 * A143439  Triangle whose n-th row is the expansion of x*(x^n*(x - 1) +
 *          (-1)^n*2).
 * https://oeis.org/A143439
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) is the entry in the integer triangle whose n‑th row
 *            (counting with offset –1) has r+2 entries for row index r. For r
 *            >= 2 the row contains only three non‑zero terms: column 0 : 0
 *            column 1 : 2·(–1)^(r+1) column r : –1 column r+1 : 1 Row 0 is
 *            –1,–1 and row 1 is 0,1,1. The linear index n is con...
 * Range:     i8 n <= 88, i16 n <= 88, i32 n <= 88, i64 n <= 88, i128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* Helper: linear start index of row r (offset –1).  */
static idx_t offset_row(idx_t r)
{
    /* r*(r+3)/2 fits in 128 bits for all feasible r.  */
    return (idx_t)(-1 + ((__int128)r * (r + 3)) / 2);
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    /* Handle the first two entries of the triangle.  */
    if (n == -1 || n == 0)
        return (seq_t)-1;

    /* Find the row r that contains n.  */
    double d  = sqrt((double)(9 + 8 * ((n >= 0) ? (n + 1) : 0)));
    idx_t r   = (idx_t)((d - 3.0) / 2.0);
    if (r < 0) r = 0;

    idx_t off = offset_row(r);
    /* Correct possible rounding errors.  */
    while (off > n) {
        --r;
        off = offset_row(r);
    }
    while (offset_row(r + 1) <= n) {
        ++r;
        off = offset_row(r);
    }

    idx_t c = n - off;                 /* column index in the row */

    /* Compute the entry according to the row.  */
    if (r == 0)
        return (seq_t)-1;               /* row 0: –1, –1  */
    else if (r == 1) {
        if (c == 0) return (seq_t)0;
        /* c == 1 or 2  */
        return (seq_t)1;
    } else { /* r >= 2 */
        if (c == 0) return (seq_t)0;
        if (c == 1) {
            /* 2·(–1)^(r+1)  */
            seq_t val = (r & 1) ? (seq_t)2 : (seq_t)-2;
            return val;
        }
        if (c == (idx_t)r) return (seq_t)-1;
        if (c == (idx_t)(r + 1)) return (seq_t)1;
        return (seq_t)0;                /* all other positions are zero  */
    }
}
