/*
 * A155124  Triangle T(n, k) = 1-n if k=0 else 2, read by rows.
 * https://oeis.org/A155124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy, less
 * Formula:   not stated
 * Range:     i8 n <= 1325, i16 n <= 1325, i32 n <= 1325, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A155124 – Triangle read by rows  
 *
 *  The triangle is defined by  
 *      T(n,k) = 1 - n  if k == 0  
 *             2        otherwise
 *  The sequence a(n) is obtained by reading this triangle row by row.
 *  
 *  For an index n the row number r is the largest integer with  
 *      r(r+1)/2  ≤  n
 *  The offset of that row is offset = r(r+1)/2.  
 *  If n == offset then a(n) = 1 - r, otherwise a(n) = 2.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Binary search for r: the row containing index n */
    idx_t low  = 0;
    idx_t high = n + 1;   /* high is exclusive bound */

    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t r = low;       /* row number */

    /* offset of row r */
    idx_t offset = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - offset;   /* column within the row */

    if (k == 0)
        return (seq_t)1 - (seq_t)r;   /* first element of the row */
    else
        return (seq_t)2;              /* all other elements */
}
