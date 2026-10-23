/*
 * A134544  Triangle read by rows: T(n,k) = Sum_{d|n, d>=k} k.
 * https://oeis.org/A134544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the triangle read by rows, where T(n,k) = k * (# of
 *            divisors of n that are ≥ k). The sequence a(n) lists T(1,1),
 *            T(2,1),T(2,2),T(3,1),T(3,2),T(3,3), …
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   count_ge_div
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_ge_div.h"

static idx_t row_of(idx_t n) {
    /* binary search for the smallest r such that r*(r+1)/2 >= n */
    idx_t low = 1, high = n;
    idx_t ans = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n) {
            ans = mid;
            if (mid == 0) break;
            if (mid == 0) break;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return ans;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* out of bounds, safe return */
    idx_t r = row_of(n);                /* row index */
    idx_t prev = (idx_t)((__int128)(r - 1) * r / 2); /* terms before this row */
    idx_t k = n - prev;                 /* column index (1‑based) */
    idx_t cnt = count_ge_div(r, k);
    return (seq_t)k * (seq_t)cnt;
}
