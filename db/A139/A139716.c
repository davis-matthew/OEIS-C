/*
 * A139716  If k is the largest divisor of n that is <= sqrt(n) then a(n) = n
 *          - k^2.
 * https://oeis.org/A139716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - k² where k is the largest divisor of n that is ≤ √n
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_u64(idx_t n)
{
    /* Binary search for floor(sqrt(n)).  Use 128‑bit multiplication
       to avoid overflow when checking mid². */
    idx_t lo = 0, hi = n, mid, res = 0;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        /* mid*mid may overflow uint64_t; use unsigned __int128. */
        if ((unsigned __int128)mid * (unsigned __int128)mid <= n) {
            res = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Compute the largest divisor k ≤ √n. */
    idx_t k = isqrt_u64(n);
    for (idx_t d = k; d > 0; --d) {
        if (n % d == 0) {
            k = d;
            break;
        }
    }
    /* Compute n - k² in seq_t. */
    seq_t kk = (seq_t)k * (seq_t)k;
    return (seq_t)n - kk;
}
