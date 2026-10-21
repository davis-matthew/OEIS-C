/*
 * A085841  Triangle: row #n has n+1 terms. T(n,m) = 4^m (2n+1)! / ( (2n-2m)!
 *          (2m+1)! ).
 * https://oeis.org/A085841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 4^m * binom(2r+1, 2m+1) * where r =
 *            floor((sqrt(8*n+1)-1)/2) and * m = n - r*(r+1)/2 (0‑based column
 *            within row r)
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r and the cumulative number of terms before that row. */
    idx_t r = 0;
    idx_t begin = 0;                     /* number of terms before row r */

    /* Keep adding the length of each row until adding the next row would
     * exceed n.  The loop runs at most O(√n) times. */
    while (begin + r + 1 <= n) {
        begin += r + 1;                  /* length of current row (r+1) */
        r++;
    }

    /* The position within the row */
    idx_t m = n - begin;                 /* 0‑based column index */

    /* Compute 4^m iteratively.  Each intermediate value fits in seq_t
     * because the final product does. */
    seq_t pow4 = 1;
    for (idx_t i = 0; i < m; ++i) {
        pow4 *= 4;
    }

    /* Binomial coefficient (2r+1 choose 2m+1) */
    seq_t bin_val = binom(2 * r + 1, 2 * m + 1);

    return bin_val * pow4;
}
