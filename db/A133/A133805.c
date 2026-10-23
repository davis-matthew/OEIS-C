/*
 * A133805  Triangle read by rows: A007318 * A133566 * A133080 as infinite
 *          lower triangular matrices.
 * https://oeis.org/A133805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r,k) + binomial(r,k+1) + * ((k % 2 == 0) ?
 *            binomial(r,k+2) : 0) * where r = floor( (sqrt(8*n+1) - 1)/2 )
 *            and k = n - r*(r+1)/2 * (r is the row index, k is the column
 *            index in that row)
 * Range:     u8 n <= 37, u16 n <= 159, u32 n <= 607, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
static idx_t
find_row(idx_t n)
{
    idx_t low = 0, high = n;          /* r <= n always */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1) / 2;
        if (prod <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t
a(idx_t n)
{
    /* Determine the (row,column) pair for index n */
    idx_t r = find_row(n);
    unsigned __int128 base = (unsigned __int128)r * (r + 1) / 2;   /* r*(r+1)/2  */
    idx_t k = n - (idx_t)base;                                   /* column    */

    seq_t val = binomial(r, k)                                   /* C(r,k)   */
             + binomial(r, k + 1);                               /* + C(r,k+1) */

    if ((k & 1) == 0)                                            /* k even?  */
        val += binomial(r, k + 2);                               /* + C(r,k+2) */

    return val;
}
