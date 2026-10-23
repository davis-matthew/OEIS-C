/*
 * A122798  A P_5-stuttered arithmetic progression with a(n+1) = a(n) if n is
 *          a pentagonal number, a(n+1) = a(n)+4 otherwise.
 * https://oeis.org/A122798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n - 3 - 4*floor((1+sqrt(1+24*(n-1)))/6)
 * Range:     u8 n <= 71, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 64‑bit unsigned integer.
   Returns the largest k such that k*k <= n. */
static inline idx_t isqrt_u64(idx_t n)
{
    if (n == 0ULL || n == 1ULL)
        return n;
    idx_t x = n;
    idx_t y = (x >> 1) + 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for the OEIS offset (1) */

    /* Count how many pentagonal numbers are <= n-1 */
    idx_t m = n - 1;                  /* upper bound for pentagonal indices 1..m */
    idx_t t = 1 + 24 * m;             /* the argument inside the sqrt */
    idx_t r = isqrt_u64(t);           /* floor( sqrt(1+24*(n-1)) ) */
    idx_t pent = (1 + r) / 6;         /* number of pentagonal numbers <= n-1 */

    /* Compute a(n) = 4*n - 3 - 4*pent */
    seq_t val = 4u * (seq_t)n - 3u;
    val -= 4u * (seq_t)pent;
    return val;
}
