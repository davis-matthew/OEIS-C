/*
 * A034096  Fractional part of square root of n starts with digit 0 (squares
 *          excluded).
 * https://oeis.org/A034096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the n‑th integer k such that the fractional part of
 *            sqrt(k) starts with digit 0 * (i.e. k lies strictly between m²
 *            and (m+0.1)² for some integer m), excluding perfect squares. *
 *            For each m the count of such integers is floor(m/5). The nth
 *            such integer is * a(n) = m² + (n − T(m−1)), where...
 * Range:     u8 n <= 18, u16 n <= 6426, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   T_of_m
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/T_of_m.h"

seq_t a(idx_t n)
{
    /* Find the smallest m such that T(m) >= n via exponential search + binary search */
    idx_t lo = 0, hi = 1;
    while (T_of_m(hi) < n) {
        hi <<= 1;
        if (hi == 0) { /* overflow, set to maximum idx_t */
            hi = UINT64_MAX;
            break;
        }
    }
    while (lo + 1 < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (T_of_m(mid) >= n)
            hi = mid;
        else
            lo = mid;
    }
    idx_t m = hi;                 /* block index containing the nth element */
    idx_t prev = T_of_m(m - 1);   /* cumulative count before this block */
    idx_t offset = n - prev;      /* 1‑based position within this block */

    /* Compute a(n) = m² + offset */
    seq_t val = (seq_t)m * (seq_t)m + (seq_t)offset;
    return val;
}
