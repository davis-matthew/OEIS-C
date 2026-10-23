/*
 * A101447  Triangle read by rows: T(n,k) = (2*k+1)*(n+1-k), 0 <= k < n.
 * https://oeis.org/A101447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   The triangle T(n,k) = (2*k+1)*(n+1-k) is read by rows. For a
 *            global index n (a(0)=first term), let r be the row such that
 *            r(r+1)/2 ≤ n < (r+1)(r+2)/2 and let k = n - r(r+1)/2. Then a(n)
 *            = (2*k+1)*(r+1-k)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   row_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/row_of.h"

seq_t a(idx_t n)
{
    idx_t r   = row_of(n);
    idx_t base = r * (r + 1) / 2;     /* number of terms before row r */
    idx_t k   = n - base;             /* column index within row r */

    /* Compute term: (2*k+1)*(r+1-k) */
    seq_t first = (seq_t)(2 * k + 1);
    seq_t second = (seq_t)(r + 1 - k);
    return first * second;
}
