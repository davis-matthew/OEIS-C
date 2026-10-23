/*
 * A176088  Table T(n,k) = ceiling(10^n/(10^k-1)), n >= 0, k >= 1, read by
 *          antidiagonals.
 * https://oeis.org/A176088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 54, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176088
 *
 * Table T(n,k) = ceil(10^n / (10^k - 1)),  n >= 0, k >= 1.
 * a(n) is obtained by reading the table by antidiagonals:
 *
 *   a(0) = T(0,1)
 *   a(1) = T(1,1)
 *   a(2) = T(0,2)
 *   a(3) = T(2,1)
 *   a(4) = T(1,2)
 *   a(5) = T(0,3)
 *   …
 *
 * For an index n the unique diagonal d (1‑based) satisfies
 *   d(d+1)/2 > n ≥ (d-1)d/2.
 * Let offset = n - (d-1)d/2.
 * Then the coordinates are
 *   row = d-1 - offset,   col = 1 + offset.
 *
 * The function implements this mapping and evaluates
 *   T(row,col) = ceil(10^row / (10^col - 1))
 * using big‑integer arithmetic provided by the build.
 */

#include <stdint.h>
#include "../utils/pow10.h"

static inline idx_t find_diag(idx_t n)
{
    /* binary search for smallest d with d(d+1)/2 > n */
    idx_t low = 1, high = n + 1;   /* upper bound n+1 is always high enough */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 val = (__int128)mid * (mid + 1) / 2;
        if (val > n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine coordinates (row, col) in the infinite table. */
    idx_t d = find_diag(n);                              /* diagonal number */
    idx_t prev = (idx_t)(((__int128)(d - 1) * d) / 2);  /* total elements before this diag */
    idx_t offset = n - prev;                              /* 0‑based offset inside the diagonal */

    idx_t row = d - 1 - offset;  /* 0‑based row index */
    idx_t col = 1 + offset;      /* column index starts from 1 */

    /* Special cases that avoid big integer arithmetic. */
    if (col > row)
        return (seq_t)1;             /* T(i,k)=1 for k>i */
    if (col == row)
        return (seq_t)2;             /* T(i,i)=2 for i>0; for i=0 handled below */

    /* General case: ceil(10^row / (10^col - 1)). */
    seq_t num = pow10(row);                    /* 10^row      */
    seq_t den = pow10(col);                    /* 10^col      */
    den -= (seq_t)1;                           /* 10^col - 1  */

    seq_t q = num / den;                       /* floor division */
    seq_t r = num % den;                       /* remainder    */
    if (r != (seq_t)0)
        ++q;                                  /* add 1 for ceil */

    return q;
}
