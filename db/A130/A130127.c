/*
 * A130127  Triangle defined by A000012 * A130125, read by rows.
 * https://oeis.org/A130127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   for n ≥ 1, a(n) = 2^(k−1) * floor((r−k+2)/2) where r = smallest
 *            integer with r(r+1)/2 ≥ n and k = n − r(r−1)/2 (1 ≤ k ≤ r)
 * Range:     u8 n <= 44, u16 n <= 152, u32 n <= 560, u64 n <= 2144,
 *            u128 n <= 5050, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>            /* for uint64_t if needed */
#include "../utils/pow2.h"     /* seq_t pow2(idx_t) */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for indices below the offset */

    /* locate the row r by binary search on triangular numbers */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* offset of first element in this row (1‑based index of row r) */
    idx_t offset = (r - 1) * r / 2;

    /* position k within the row (1‑based) */
    idx_t k = n - offset;          /* 1 ≤ k ≤ r */

    /* power = 2^(k−1) using the provided helper */
    seq_t power = pow2(k - 1);

    /* factor = floor((r−k+2)/2) */
    idx_t factor = (r - k + 2) >> 1;

    return power * (seq_t)factor;
}
