/*
 * A095837  Triangle read by rows: T(n,k) = (n-k+1)^((n-k)^2), n>=1, 1<=k<=n.
 * https://oeis.org/A095837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (r - k + 1)^( (r - k)^2 ) where r is the row containing
 *            index n in the triangle, and k is the column within that row.
 *            The triangle entry T(r,k) = (r - k + 1)^( (r - k)^2 ). The
 *            sequence a(n) is the triangle read by rows
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 10, u64 n <= 15, u128 n <= 24,
 *            big any n
 * Helpers:   _pow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_pow_seq_t.h"

seq_t a(idx_t n)
{
    /* Find the row r such that the cumulative
       number of terms up to row r-1 is < n <= r*(r+1)/2. */
    idx_t r = 1;
    while (1) {
        __int128 cum = (__int128)r * (r + 1) / 2;
        if ((idx_t)cum >= n) break;
        ++r;
    }

    /* Compute position within the row (k) */
    __int128 prev = (__int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev;           /* 1 <= k <= r */

    /* Triangle value: base = r-k+1, exponent = (r-k)^2 */
    idx_t base_int = r - k + 1;
    idx_t diff     = r - k;
    idx_t exp      = diff * diff;        /* exponent fits in idx_t for all practical n */

    seq_t base = (seq_t)base_int;
    seq_t result = _pow_seq_t(base, exp);

    return result;
}
