/*
 * A117724  Triangle T(n,k) = coefficient [x^n] of x^2/(1-(k+1)*x^2-x^3) for
 *          row n, and columns k = 0..n, read by rows.
 * https://oeis.org/A117724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 86, u32 n <= 185, u64 n <= 487,
 *            u128 n <= 1222, big any n
 * Helpers:   offset_row_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Triangle read by rows.
   Define T(n,k) by the recurrence

        T(0,k) = 0,   T(1,k) = 0,   T(2,k) = 1
        T(n,k) = (k+1) * T(n-2,k) + T(n-3,k)    for n >= 3

   The sequence a(n) is obtained by reading T(n,k) row by row:
   find r,k such that n = r(r+1)/2 + k (0 ≤ k ≤ r) and
   a(n) = T(r,k).
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/offset_row_val.h"

/* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
static inline idx_t row_of(idx_t n)
{
    idx_t lo = 0, hi = 1;
    while (offset_row_val(hi) <= n) hi <<= 1;          /* exponential search */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if (offset_row_val(mid) <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    idx_t r = row_of(n);                        /* row index */
    idx_t off = (idx_t)offset_row_val(r);       /* offset of the row */
    idx_t k = n - off;                          /* column index */

    /* Base cases */
    if (r < 2) return 0;
    if (r == 2) return 1;

    /* Allocate small array to compute recurrence for this k. */
    seq_t *T = (seq_t *)malloc((size_t)(r + 1) * sizeof(seq_t));
    if (!T) return 0;    /* allocation failure fallback */

    T[0] = 0;
    T[1] = 0;
    T[2] = 1;

    for (idx_t i = 3; i <= r; ++i) {
        T[i] = (seq_t)(k + 1) * T[i - 2] + T[i - 3];
    }

    seq_t result = T[r];
    free(T);
    return result;
}
