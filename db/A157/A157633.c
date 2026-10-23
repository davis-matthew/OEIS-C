/*
 * A157633  Triangle T(n,m) read rows: 1 in column m=0 and on the diagonal,
 *          2*m*(n-m)*(m^2-n*m+2*n^2) otherwise.
 * https://oeis.org/A157633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A157633 – triangle T(n,m) read by rows (0‑based index).
 * T(r,m) = 1                              if m == 0 or m == r
 * T(r,m) = r^4 - m^4 - (r-m)^4            otherwise
 * a(n) is the nth element of this array, where the first element of
 * row r starts at linear index r*(r+1)/2.
 */

#include <stdint.h>

/* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.  */
static inline idx_t
row_of(idx_t n)
{
    idx_t low = 0, high = 1;

    /* Grow high until it is past n. */
    while ((high * (high + 1)) / 2 <= n)
        high <<= 1;

    /* Binary search on [low, high).  */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((mid * (mid + 1)) / 2 <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;      /* low is the first row with offset > n */
}

/* Main sequence function.  */
seq_t
a(idx_t n)
{
    /* Locate row and column within that row. */
    idx_t r = row_of(n);
    idx_t offset = (r * (r + 1)) / 2;
    idx_t m = n - offset;   /* column index in the row */

    /* Border values are 1. */
    if (m == 0 || m == r)
        return (seq_t)1;

    /* Compute interior value safely in a wide integer type. */
    unsigned __int128 nr = (unsigned __int128)r;
    unsigned __int128 mr = (unsigned __int128)m;
    unsigned __int128 term = nr * nr * nr * nr
                          - mr * mr * mr * mr
                          - (nr - mr) * (nr - mr) * (nr - mr) * (nr - mr);

    return (seq_t)term;
}
