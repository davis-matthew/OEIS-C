/*
 * A140649  Triangle whose rows are decreasing powers of 2, followed by 0.
 * https://oeis.org/A140649
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = T(r, k) where * T(r,k) = 2^(r-k-1) if r > k, otherwise 0,
 *            * r is the unique integer such that * start_r = 2 + (r-1)(r+2)/2
 *            <= n < start_{r+1}, * and k = n - start_r. * This enumerates the
 *            rows of decreasing powers of 2 followed by a 0
 * Range:     u8 n <= 45, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   _row_from_idx, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/_row_from_idx.h"

seq_t a(idx_t n) {
    if (n < 2)
        return (seq_t)0;          /* off‑offset values are undefined */

    idx_t r = _row_from_idx(n);

    /* Compute the linear starting index of this row */
    unsigned __int128 start128 = 2 + (unsigned __int128)(r - 1) * (r + 2) / 2;
    idx_t start = (idx_t)start128;

    idx_t k = n - start;           /* column index within the row */

    if (r <= k)                    /* zero at the end of the row */
        return (seq_t)0;

    idx_t exp = r - k - 1;         /* exponent for the power of 2 */
    return pow2(exp);
}
