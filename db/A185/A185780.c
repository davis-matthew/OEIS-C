/*
 * A185780  Array T(n,k) = k*(n*k-n+1), by antidiagonals.
 * https://oeis.org/A185780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) enumerates T(n,k) = k*(n*k-n+1) by antidiagonals
 * Range:     u8 n <= 68, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 64‑bit unsigned integer, using binary search. */
static inline idx_t isqrt_u64(idx_t x) {
    idx_t left = 0, right = x;
    while (left <= right) {
        idx_t mid = left + ((right - left) >> 1);
        if (mid > 0 && mid > x / mid) {
            if (mid == 0) break;
            right = mid - 1;
        } else {
            if ((mid + 1) > 0 && (mid + 1) > x / (mid + 1)) {
                return mid;
            }
            left = mid + 1;
        }
    }
    return 0;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1, but safeguard */
    /* Find t such that t*(t+1)/2 >= n */
    idx_t t = (isqrt_u64(8 * n + 1) - 1) >> 1;
    if (t * (t + 1) / 2 < n) t++;

    idx_t cum_prev = t * (t - 1) / 2;   /* terms before current block */
    idx_t pos = n - cum_prev;           /* 1‑based position inside block */

    idx_t k = t - pos + 1;              /* column index */
    idx_t s = t + 1;                    /* row + column sum */
    idx_t row_n = s - k;                /* row index */

    /* Compute T(row_n, k) = k*(row_n*k - row_n + 1) */
    seq_t val = (seq_t)k * ((seq_t)row_n * k - row_n + 1);
    return val;
}
