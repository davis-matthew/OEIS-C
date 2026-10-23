/*
 * A174806  a(n) = n-floor(sqrt(n))^2-floor(sqrt(n-floor(sqrt(n))^2))^2;
 *          difference between n and sum of two largest distinct squares <= n.
 * https://oeis.org/A174806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - floor(sqrt(n))^2 - floor(sqrt(n -
 *            floor(sqrt(n))^2))^2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for unsigned 64‑bit values.
   Returns the largest r such that r*r <= n.  */
static inline idx_t isqrt_uint64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        idx_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n) low = mid + 1;
        else high = mid - 1;
    }
    return high;  /* high is floor(sqrt(n)) */
}

seq_t a(idx_t n)
{
    idx_t r1 = isqrt_uint64(n);                /* floor(sqrt(n))          */
    seq_t sq1 = (seq_t)r1 * (seq_t)r1;         /* r1^2                    */

    idx_t rest = n - r1 * r1;                  /* n - r1^2 (fits in idx_t) */
    idx_t r2     = isqrt_uint64(rest);         /* floor(sqrt(rest))       */
    seq_t sq2 = (seq_t)r2 * (seq_t)r2;         /* r2^2                    */

    return (seq_t)n - sq1 - sq2;               /* final value */
}
