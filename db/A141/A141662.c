/*
 * A141662  Triangle read by rows, T(n,m) = abs(n - m^2).
 * https://oeis.org/A141662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = |r - m^2| where r is the row index such that r*(r+1)/2 ≤
 *            n < (r+1)*(r+2)/2 and m = n - r*(r+1)/2 (column)
 * Range:     u8 n <= 169, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Determine r by binary search on triangular numbers.  Use
       unsigned __int128 to avoid overflow of r*(r+1)/2. */
    idx_t low = 0, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 <= n)
        high <<= 1;

    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned __int128 tmid = (unsigned __int128)mid * (mid + 1) / 2;
        if (tmid > n)
            high = mid - 1;
        else
            low = mid;
    }
    idx_t r = low;

    /* Offset of the first element of row r. */
    idx_t offset = (idx_t)((unsigned __int128)r * (r + 1) / 2);

    /* Column index within the row. */
    idx_t m = n - offset;

    /* Square of m (may exceed 64 bits, use __int128). */
    unsigned __int128 msq = (unsigned __int128)m * m;

    /* Absolute difference |r - m^2| as unsigned __int128. */
    unsigned __int128 diff128 = (r >= msq) ? (unsigned __int128)r - msq
                                            : msq - (unsigned __int128)r;

    return (seq_t)diff128;
}
