/*
 * A143259  a(n) = 1 if n is a nonzero square, -1 if n is twice a nonzero
 *          square, 0 otherwise.
 * https://oeis.org/A143259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = 1 if n is a nonzero square, -1 if n is twice a nonzero
 *            square, 0 otherwise. (OEIS A143259)
 * Range:     i8 n <= 9646, i16 n <= 9646, i32 n <= 9646, i64 n <= 9646,
 *            i128 n <= 9646, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer floor square root for uint64_t, safe for all values. */
static inline idx_t floor_isqrt_u64(idx_t x)
{
    if (x <= 1) return x;               /* 0 or 1 */
    idx_t lo = 1;
    idx_t hi = x;
    /* The largest possible integer square root of a 64‑bit number is 0xFFFFFFFF. */
    if (hi > 0xFFFFFFFFULL) hi = 0xFFFFFFFFULL;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        idx_t sq  = mid * mid;          /* safe: mid <= 0xFFFFFFFF */
        if (sq == x) return mid;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return lo - 1;                     /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not used for OEIS offset 1 */

    /* check for a non‑zero perfect square */
    idx_t m = floor_isqrt_u64(n);
    if (m * m == n) return (seq_t)1;

    /* check for twice a perfect square */
    if ((n & 1ULL) == 0) {
        idx_t half = n >> 1;
        idx_t t = floor_isqrt_u64(half);
        if (t * t == half) return (seq_t)-1;
    }

    return (seq_t)0;
}
