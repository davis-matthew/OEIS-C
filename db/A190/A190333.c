/*
 * A190333  n + [n*r/s] + [n*t/s]; r=1, s=sqrt(3), t=1/s.
 * https://oeis.org/A190333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n/√3⌋ + ⌊n/3⌋ (OEIS A190333)
 * Range:     u8 n <= 134, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 64‑bit unsigned integer.
   Returns the largest x with x*x ≤ n. */
static inline idx_t isqrt_idx(idx_t n)
{
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        __int128 sq = (__int128)mid * mid;
        if (sq <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;      /* avoid underflow */
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Compute ⌊n/3⌋ */
    idx_t thirds = n / 3;

    /* Compute ⌊n/√3⌋ as floor( sqrt( n² / 3 ) )  */
    idx_t n2 = n * n;                     /* n² fits in idx_t for calls used */
    idx_t t  = n2 / 3;                     /* floor(n²/3)                         */
    idx_t root = isqrt_idx(t);             /* floor(sqrt(floor(n²/3)))            */

    /* Sum the three components */
    idx_t res = n + root + thirds;         /* intermediate high precision */

    /* Convert to the sequence type (truncates if overflows) */
    return (seq_t)res;
}
