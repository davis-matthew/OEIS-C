/*
 * A144257  Weight array of A086270.
 * https://oeis.org/A144257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the value at (i,j) in the Cantor enumeration of pairs
 *            (i,j) of positive integers, where the matrix is defined as
 *            a[1][j] = j, a[i][1] = 0 for i > 1, a[i][j] = j-1 for i > 1 and
 *            j > 1. The enumeration is (1,1), (1,2),(2,1), (1,3),(2,2),(3,1),
 *            … (anti‑diagonals)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    /* Compute the sum s = i + j of the coordinates on the diagonal containing */
    /* position n.  s satisfies (s-2)*(s-1)/2 < n <= (s-1)*s/2.  */
    idx_t x = (idx_t)(((__uint128_t)8 * n) + 1);
    idx_t sq = floor_isqrt_u64(x);     /* floor(sqrt(x)) */
    idx_t t  = (sq - 1) / 2;           /* floor((sqrt(8n+1)-1)/2) */
    idx_t cum = (idx_t)(((__uint128_t)t * (t + 1)) / 2);
    idx_t s;
    if (cum >= n)
        s = t + 1;
    else
        s = t + 2;

    /* Position of n within diagonal s */
    idx_t prev   = (idx_t)(((__uint128_t)(s - 2) * (s - 1)) / 2);
    idx_t offset = n - prev - 1;            /* 0‑based offset on this diagonal */
    idx_t i = offset + 1;
    idx_t j = s - i;

    /* Return the matrix value at (i,j) as defined above */
    if (j == 1) {
        return (i == 1) ? (seq_t)1 : (seq_t)0;
    } else {
        return (i == 1) ? (seq_t)j : (seq_t)(j - 1);
    }
}
