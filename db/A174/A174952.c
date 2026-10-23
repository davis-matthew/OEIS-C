/*
 * A174952  Triangle t(n,m)= binomial(n+m-1,n-1) + binomial(2*n-m-1,n-1)
 *          -binomial(2*n-1,n-1) read by rows, 0<=m<=n.
 * https://oeis.org/A174952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = t(r,m) where t(r,m) = binomial(r+m-1,r-1) +
 *            binomial(2*r-m-1,r-1) - binomial(2*r-1,r-1) and n = r*(r+1)/2 +
 *            m (0‑based). This reads the triangle row‑wise
 * Range:     i8 n <= 15, i16 n <= 36, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Row 0 is a single 1. */
    if (n == 0)
        return (seq_t)1;

    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t low = 0;
    idx_t high = 1;

    /* Increase high until it exceeds the index. */
    while (((unsigned __int128)(high) * (high + 1)) / 2 <= n)
        high <<= 1;           /* double high */

    /* Binary search for the correct row. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 val = ((unsigned __int128)(mid) * (mid + 1)) / 2;
        if (val > n)
            high = mid;
        else
            low = mid + 1;
    }

    /* The row number is r = high - 1. */
    idx_t r = high - 1;

    /* Starting index of row r. */
    unsigned __int128 start128 = ((unsigned __int128)r * (r + 1)) / 2;
    idx_t start = (idx_t)start128;

    /* Position within the row. */
    idx_t m = n - start;

    /* For r > 0 compute the triangle entry. */
    seq_t term = binomial(r + m - 1, r - 1);
    term += binomial(2 * r - m - 1, r - 1);
    term -= binomial(2 * r - 1, r - 1);

    return term;
}
