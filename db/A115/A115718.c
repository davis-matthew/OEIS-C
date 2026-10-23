/*
 * A115718  Inverse of number triangle A115717; a divide-and-conquer related
 *          triangle.
 * https://oeis.org/A115718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 1325, i16 n <= 1325, i32 n <= 1325, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A115718 – Inverse of the number triangle A115717.
 *  For a flattened index n: find the row r such that
 *      r*(r+1)/2 ≤ n < (r+1)*(r+2)/2,
 *  and the column c = n – r*(r+1)/2.
 *
 *  The inverse triangle satisfies:
 *      if r is even:  first r/2 entries = -3, the rest = 1
 *      if r is odd :  first r entries   = 0, the last = 1
 *
 *  Thus
 *      a(n) = -3  if (r even) && (c < r/2)
 *           =  1  if (r even) && (c ≥ r/2)
 *           =  0  if (r odd ) && (c < r)
 *           =  1  if (r odd ) && (c == r)
 */

#include <stdint.h>

static inline idx_t
find_row(idx_t n)
{
    idx_t low = 0, high = n + 1;          /* high is exclusive */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    idx_t start = r * (r + 1) / 2;
    idx_t c = n - start;

    if ((r & 1) == 0) {                      /* even row */
        idx_t half = r / 2;
        if (c < half)
            return (seq_t)-3;
        else
            return (seq_t)1;
    } else {                                 /* odd row */
        if (c < r)
            return (seq_t)0;
        else
            return (seq_t)1;
    }
}
