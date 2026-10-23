/*
 * A105125  Triangle read by rows: T(n,k) = n^3 + k^3, n >= 0, 0 <= k <= n.
 * https://oeis.org/A105125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = r^3 + k^3 where r = row_of_index(n) and k = n – r(r+1)/2
 * Range:     u8 n <= 24, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   row_of_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/row_of_index.h"

seq_t a(idx_t n)
{
    /* find the row r so that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = row_of_index(n);

    /* compute k = offset within the row */
    unsigned __int128 tri = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)tri;

    /* compute the value r³ + k³ using 128‑bit intermediates to avoid overflow */
    seq_t rr = (seq_t)r;
    seq_t kk = (seq_t)k;
    seq_t value = (seq_t)((unsigned __int128)rr * rr * rr +
                         (unsigned __int128)kk * kk * kk);
    return value;
}
