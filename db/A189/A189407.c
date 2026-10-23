/*
 * A189407  n+[nr/t]+[ns/t]; r=1, s=-1+sqrt(3), t=1+sqrt(3).
 * https://oeis.org/A189407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*(√3-1)/2) + floor(n*(2-√3))
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived from a(n) = n + floor(n*r/t) + floor(n*s/t) with
   r = 1, s = -1+√3, t = 1+√3.  */

/* Helper: integer square root for 64‑bit unsigned values
   (works because 3*n*n < 3*10^8 for the requested n). */
#include <stdint.h>
static inline uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t lo = 0, hi = (x <= 0xFFFFFFFFULL) ? (uint32_t)x : 0xFFFFFFFFULL;
    uint64_t ans = 0;
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        if (mid == 0) { ans = 0; lo = 1; continue; }
        if (mid <= x / mid) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, but guard for safety */

    uint64_t nn = (uint64_t)n;
    /* floor(n*sqrt(3)) = floor(sqrt(3*n^2)) */
    uint64_t m = isqrt_uint64(3ULL * nn * nn);

    uint64_t diff = m - nn;               /* m - n  (>=0)               */
    uint64_t term1 = diff >> 1;           /* floor((m-n)/2)             */
    uint64_t term2 = 2ULL * nn - m - 1ULL;/* 2*n - m - 1                */

    return (seq_t)n + (seq_t)term1 + (seq_t)term2;
}
