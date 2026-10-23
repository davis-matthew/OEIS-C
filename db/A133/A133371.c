/*
 * A133371  Triangle read by rows: T(i,j) is the number of i-permutations of
 *          14 objects a,b,c,d,e,f,g,h,i,j,k,l,m,n, with repetition allowed,
 *          containing j a's.
 * https://oeis.org/A133371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,j) where T(r,j) = C(r,j) * 13^(r-j) and the triangle
 *            is read row by row. Here r is the row index (starting from 0)
 *            and j the column index (starting from 0)
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binom_n_k, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_n_k.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* Determine the row r such that offset_row(r) <= n < offset_row(r+1). */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t row_start = r * (r + 1) / 2;
    idx_t j = n - row_start;          /* column within the row */

    seq_t comb  = binom_n_k(r, j);           /* C(r, j)   */
    seq_t power = pow_int((seq_t)13, r - j); /* 13^(r-j) */

    return comb * power;
}
