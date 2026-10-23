/*
 * A133807  Triangle read by rows: A007318 * (A097806 + A133566 - I) as
 *          infinite lower triangular matrices, where I is the identity
 *          matrix.
 * https://oeis.org/A133807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 158, u32 n <= 608, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows: a(n) = T(i,j) where
   i = floor( ( sqrt(8n + 1) - 1 ) / 2 )
   j = n - i(i+1)/2
   and T(i,j) = binom(i,j) + ( 3·binom(i,j+1) - (−1)^j·binom(i,j+1) ) / 2,
   which simplifies to
   T(i,j) = binom(i,j) + binom(i,j+1)          if j even
            binom(i,j) + 2·binom(i,j+1)        if j odd   */

#include <stdint.h>
#include "../utils/binomial.h"

static idx_t _row_of(idx_t n)
{
    /* Find the largest r such that r(r+1)/2 <= n  */
    idx_t low = 0, high = 1;

    /* Grow high until it exceeds the required row, but keep it in range */
    while (((__int128)high * (high + 1)) / 2 <= n)
    {
        if (high > (UINT64_MAX >> 1)) {   /* avoid overflow on shifting */
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }

    /* Binary search for the exact row */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (((__int128)mid * (mid + 1)) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t r = _row_of(n);                            /* row index */
    idx_t row_start = (idx_t)(((__int128)r * (r + 1)) / 2);
    idx_t k = n - row_start;                         /* column index */

    seq_t b0 = binomial(r, k);                       /* binom(r,k)  */
    seq_t b1 = (k + 1 <= r) ? binomial(r, k + 1) : 0; /* binom(r,k+1) */

    if ((k & 1) == 0)                               /* even k      */
        return b0 + b1;                              /* + binom(r,k+1)  */
    else                                             /* odd k       */
        return b0 + (seq_t)2 * b1;                   /* + 2·binom(r,k+1)*/
}
