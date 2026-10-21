/*
 * A080511  Triangle whose n-th row contains the least set (ordered
 *          lexicographically) of n distinct positive integers whose
 *          arithmetic mean is an integer.
 * https://oeis.org/A080511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = { n if n is odd 3n/2 if n is even } of the nth row, where
 *            the nth row consists of the numbers {1,2,…,n‑1,x} with x as
 *            above. We flatten the triangle row by row: a(1)=1, a(2)=1,
 *            a(3)=3, a(4)=1, … (See OEIS A080511)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, but idx_t and seq_t are defined by the build */

seq_t a(idx_t n) {
    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t low = 0, high = n;           /* r <= n for n>=1 */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n) high = mid;
        else low = mid + 1;
    }
    idx_t r = low;                     /* the row number (1‑based) */

    /* Compute the column index (1‑based) within that row */
    unsigned __int128 prev_tri = (unsigned __int128)(r - 1) * r / 2;
    idx_t col = (idx_t)(n - prev_tri);

    /* Determine the value at this position */
    if (col == r) {                    /* last element in the row */
        if ((r & 1) == 0) {            /* even r */
            return (seq_t)((seq_t)r * 3 / 2);
        } else {                       /* odd r */
            return (seq_t)r;
        }
    } else {
        return (seq_t)col;             /* one of the 1…r‑1 elements */
    }
}
