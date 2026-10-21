/*
 * A091884  Triangle of numbers defined by Knuth.
 * https://oeis.org/A091884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum_{j=0..k} (-1)^j * (r-j)^r where r =
 *            floor((sqrt(8*n+1)-1)/2) is the row index in the triangular
 *            array and k = n - r*(r+1)/2 is the column index. This implements
 *            OEIS A091884
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 54, u64 n <= 135, u128 n <= 377,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column offset within the row */
    idx_t k = n - r * (r + 1) / 2;

    seq_t sum = 0;

    for (idx_t j = 0; j <= k; ++j) {
        seq_t term = 1;
        /* Compute (r-j)^r by repeated multiplication */
        for (idx_t m = 0; m < r; ++m)
            term *= (seq_t)(r - j);

        if (j & 1)
            sum -= term;
        else
            sum += term;
    }

    return sum;
}
