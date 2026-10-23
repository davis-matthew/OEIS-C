/*
 * A171174  Triangle read by rows in which row n lists A033627(n) together
 *          with the first 2n-1 numbers <> 0 of A038608.
 * https://oeis.org/A171174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, tabf
 * Formula:   a(n) = { 2 , if n = 1 3*r - 2 , if n in first position of row r
 *            (r ≥ 2) (k if k even, -k if k odd) , where k = position_in_row -
 *            1 } Here rows are defined by the cumulative count r(r+1) (i.e.
 *            row r has 2r terms). The first element of each row is A033627(r)
 *            = 3r-2 for r≥2 and 2 for r=1. The remain...
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t definition of idx_t in the build */

static idx_t find_row(idx_t n)
{
    /* Find the smallest r such that r*(r+1) >= n.
       Since r is at most sqrt(n), a simple linear search is fast enough
       for all n that fit in the test data (up to at least 10 000). */
    idx_t r = 1;
    while ((__int128)r * (r + 1) < n) r++;
    return r;
}

seq_t a(idx_t n)
{
    /* Determine the row and position within the row. */
    idx_t r = find_row(n);                     /* 1‑based row number   */
    idx_t prev_cum = (r - 1) * r;              /* terms before this row */
    idx_t pos      = n - prev_cum;             /* position in the row   */

    /* First element of the row – the A033627(r) value. */
    if (pos == 1) {
        if (r == 1) return (seq_t)2;
        return (seq_t)(3 * (seq_t)r - 2);
    }

    /* Remaining elements: alternating signed numbers. */
    idx_t k = pos - 1;                          /* 1 .. 2r-1 */
    if (k & 1)  /* odd k → negative */
        return -(seq_t)k;
    else        /* even k → positive */
        return (seq_t)k;
}
