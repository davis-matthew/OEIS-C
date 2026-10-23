/*
 * A186346  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f(i)=8i and g(j)=j^2. Complement of
 *          A186347.
 * https://oeis.org/A186346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(sqrt(8*n - 1/2))
 * Range:     u8 n <= 112, u16 n <= 112, u32 n <= 112, u64 n <= 112,
 *            u128 n <= 112, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent integer formulation: a(n) = n + floor(sqrt(8*n - 1))   */

#include <stdint.h>   /* for standard integer types   */

static seq_t int_sqrt_u128(__int128 x)
{
    /* Return floor(sqrt(x)) for 0 <= x < 2^128 */
    __int128 lo = 0, hi = x, mid, sq;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        sq  = mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return (seq_t)hi;
}

seq_t a(idx_t n)
{
    /* a(n) = n + floor(sqrt(8*n - 1))     */
    /* Compute m = 8*n - 1 as a 128‑bit integer   */
    __int128 m = (__int128)n * 8 - 1;
    seq_t s = int_sqrt_u128(m);   /* floor sqrt of 8*n-1 */
    return (seq_t)n + s;
}
