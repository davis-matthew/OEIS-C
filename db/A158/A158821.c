/*
 * A158821  Triangle read by rows: row n (n>=0) ends with 1, and for n>=1
 *          begins with n; other entries are zero.
 * https://oeis.org/A158821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 1 if n is the last element of a row (or n = 0); * a(n) =
 *            r (row index) if n is the first element of that row and r>0; *
 *            otherwise a(n) = 0. The rows are numbered from 0 and the first *
 *            element of row r has column 0, the last element column r. * The
 *            algorithm finds the row r by binary searc...
 * Range:     u8 n <= 1325, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t find_row(idx_t n)
{
    /* return the largest r with r*(r+1)/2 <= n */
    idx_t low = 0, high = 1;
    /* grow high until it exceeds the desired range (avoiding overflow) */
    while (( (__int128)high * (high + 1) ) / 2 <= n) {
        if (high > ((idx_t)~0ULL >> 1)) break;  /* prevent infinite loop */
        high <<= 1;
    }
    /* binary search between low and high */
    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ( ( (__int128)mid * (mid + 1) ) / 2 <= n )
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)                    /* special case for the first row */
        return (seq_t)1;

    idx_t r = find_row(n);          /* row index of n */
    idx_t start = ( (__int128)r * (r + 1) ) / 2;
    idx_t col   = n - start;       /* column within row */

    if (col == 0)                 /* first element of the row (r>0) */
        return (seq_t)r;
    if (col == r)                 /* last element of the row */
        return (seq_t)1;
    return (seq_t)0;                /* all other positions are zero */
}
