/*
 * A182828  Array of the numbers (3*i+1)*3^j, i>=0, j>=0, read by
 *          antidiagonals.
 * https://oeis.org/A182828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (3*i+1) * 3^j read by antidiagonals where (i,j) pair
 *            satisfies i+j = k and 0 <= i <= k with k chosen so that
 *            k*(k+1)/2 < n <= (k+1)*(k+2)/2 (i.e. a(1) = (3*0+1)*3^0 = 1,
 *            a(2) = (3*0+1)*3^1 = 3, …)
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow3.h"

/* Find greatest integer d with d*(d+1)/2 <= m */
static idx_t
find_diagonal(idx_t m)
{
    idx_t lo = 0;
    idx_t hi = 1;
    /* Exponential search to bound hi */
    while ((unsigned __int128)hi * (hi + 1) / 2 <= m)
        hi <<= 1;
    /* Binary search between lo and hi */
    while (lo + 1 < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if ((unsigned __int128)mid * (mid + 1) / 2 <= m)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t
a(idx_t n)
{
    if (n == 0) /* offset is 1; undefined for n=0 – return 0 */
        return 0;
    idx_t m = n - 1;               /* zero‑based index */
    idx_t d = find_diagonal(m);    /* diagonal number */
    idx_t i = m - d * (d + 1) / 2; /* position in the diagonal */
    idx_t j = d - i;               /* complementary coordinate */
    seq_t val = (seq_t)(3 * i + 1) * pow3(j);
    return val;
}
