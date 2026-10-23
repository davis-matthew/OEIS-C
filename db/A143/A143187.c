/*
 * A143187  Triangle read by rows: T(n, k) = f(k) for 1 <= k <= floor(n/2),
 *          T(n, k) = f(n-k) for floor(n/2) < k <= n-1, with T(n, 0) = 1, T(n,
 *          n) = 1, and f(k) = (1/2)*(3 - (-1)^k)*k.
 * https://oeis.org/A143187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = f(min(k, r-k)), where r = floor((sqrt(8n+1)-1)/2) (row
 *            index), k = n - r(r+1)/2 (column index), f(k) = k if k even else
 *            2k. The triangle has T(r,0)=T(r,r)=1 and mirror symmetry
 * Range:     u8 n <= 1325, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   offset_row_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/offset_row_local.h"

seq_t a(idx_t n)
{
    /* Find the row r such that offset(r) ≤ n < offset(r+1). */
    idx_t low = 0, high = 1;
    while (offset_row_local(high) <= n) {
        if (high > (UINT64_MAX >> 1)) {  /* safeguard against overflow */
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }
    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (offset_row_local(mid) <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t r = low;
    idx_t offset = offset_row_local(r);
    idx_t j = n - offset;           /* column index within the row */

    if (j == 0 || j == r)          /* first or last element of the row */
        return (seq_t)1;

    idx_t k = (j <= (r >> 1)) ? j : (r - j);
    if ((k & 1U) == 0)
        return (seq_t)k;           /* even k */
    else
        return (seq_t)(k << 1);    /* odd k → 2k */
}
