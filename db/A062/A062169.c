/*
 * A062169  Triangle T(n, k) = k! mod n for n >= 1, 1 <= k <= n.
 * https://oeis.org/A062169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) where r is the unique integer with r(r-1)/2 < n
 *            <= r(r+1)/2 (i.e., the row of the triangle) and k = n - r(r-1)/2
 *            is the column in that row. T(r, k) = k! mod r. The function
 *            implements this definition
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   col_in_row, row_of_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/col_in_row.h"
#include "../utils/row_of_n.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;                 /* safety, though n>=1 per spec */

    idx_t r = row_of_n(n);               /* row number */
    idx_t k = col_in_row(n, r);          /* column number */

    seq_t mod = (seq_t)r;
    unsigned __int128 mod128 = (unsigned __int128)mod;

    seq_t prod = 1 % mod;                /* initial factorial value */
    for (idx_t j = 1; j <= k; ++j) {
        unsigned __int128 tmp = (unsigned __int128)prod * (unsigned __int128)j;
        prod = (seq_t)(tmp % mod128);
    }
    return prod;
}
