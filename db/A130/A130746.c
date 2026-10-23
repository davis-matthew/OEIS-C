/*
 * A130746  Triangle read by rows: T(n,m) = binomial(n+m,1+n), 1<=m<=n.
 * https://oeis.org/A130746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(r+m, r+1), * where r = ceil((sqrt(8*n+1)-1)/2)
 *            is the row number * such that r*(r+1)/2 >= n, and * m = n -
 *            r*(r-1)/2 is the column within the row
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binom.h"

static idx_t find_row(idx_t n)
{
    idx_t low = 1, high = 1;
    /* enlarge high until the cumulative count exceeds n */
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;
    /* binary search for the smallest r with r*(r+1)/2 >= n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* The sequence is 1‑based; values for n<1 are uninteresting. */
    if (n == 0) return 0;

    idx_t r = find_row(n);
    /* previous cumulative count before row r */
    idx_t prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t m = n - prev;             /* column index, 1‑based */

    return binom(r + m, r + 1);
}
