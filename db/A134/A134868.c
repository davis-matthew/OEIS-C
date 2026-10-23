/*
 * A134868  A103451 * A002260.
 * https://oeis.org/A134868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = f(r,c) where r is the row and c the column * - if r==1:
 *            a(1)=1 * - otherwise: a = 2 for c=1 or c=2, else a=c * The row r
 *            is the smallest integer with r*(r+1)/2 >= n, * and the column c
 *            satisfies n = (r-1)*r/2 + c
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    /* Find an upper bound for the row number r */
    idx_t low = 1, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;

    /* Binary search for the smallest r with r*(r+1)/2 >= n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 cur = (unsigned __int128)mid * (mid + 1) / 2;
        if (cur >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Compute the column index c */
    unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t c = n - (idx_t)prev;

    if (c == 1 || c == 2)
        return (seq_t)2;
    return (seq_t)c;
}
