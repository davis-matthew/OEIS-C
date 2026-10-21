/*
 * A075117  Table by antidiagonals of generalized Lucas numbers: T(n,k) =
 *          T(n,k-1) + n*T(n,k-2) with T(n,0)=2 and T(n,1)=1.
 * https://oeis.org/A075117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 139, u32 n <= 382, u64 n <= 999,
 *            u128 n <= 2864, big any n
 * Helpers:   compute_T
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS entry A075117: Table by antidiagonals of generalized Lucas numbers
 * 
 * Let T(n,k) be defined by: T(n,0)=2, T(n,1)=1, T(n,k)=T(n,k-1)+n*T(n,k-2).
 * The sequence a(n) is given by reading the table T along antidiagonals:
 *   a(n) = T(i,j)   where i = n - d*(d+1)/2,
 *                    j = d - i,
 *                    d is the unique integer satisfying d*(d+1)/2 <= n < (d+1)*(d+2)/2.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/compute_T.h"

seq_t a(idx_t n)
{
    /* Determine diagonal index d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t d = 0;
    while (((unsigned __int128)(d + 1) * (d + 2)) / 2 <= n) {
        ++d;
    }

    /* Compute offset m within this diagonal */
    idx_t prev_sum = ((unsigned __int128)d * (d + 1)) / 2; /* sum of lengths up to d-1 */
    idx_t m = n - prev_sum;          /* 0‑based index within diagonal */
    idx_t i = m;                     /* first argument of T */
    idx_t j = d - m;                 /* second argument of T */

    return compute_T(i, j);
}
