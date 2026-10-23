/*
 * A185548  a(n) = floor(floor(n^(5/2))^(2/3)).
 * https://oeis.org/A185548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(floor(n^(5/2))^(2/3))
 * Range:     u8 n <= 27, u16 n <= 776, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute floor(sqrt(x)) for an unsigned 128‑bit integer x. */
static unsigned __int128 isqrt_uint128(unsigned __int128 x)
{
    if (x == 0) return 0;
    unsigned __int128 high = 1;
    /* Find an upper bound: high^2 > x */
    while (high * high <= x) high <<= 1;
    unsigned __int128 low = high >> 1;
    while (low + 1 < high) {
        unsigned __int128 mid = (low + high) >> 1;
        if (mid * mid <= x)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* Compute floor(cubert(x)) for an unsigned 128‑bit integer x. */
static unsigned __int128 icbrt_uint128(unsigned __int128 x)
{
    if (x == 0) return 0;
    unsigned __int128 high = 1;
    /* Find an upper bound: high^3 > x */
    while (high * high * high <= x) high <<= 1;
    unsigned __int128 low = high >> 1;
    while (low + 1 < high) {
        unsigned __int128 mid = (low + high) >> 1;
        if (mid * mid * mid <= x)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    unsigned __int128 n128 = (unsigned __int128)n;
    /* Compute n^5 in 128‑bit arithmetic: n^5 = n^2 * n^2 * n */
    unsigned __int128 n2 = n128 * n128;          /* n^2   */
    unsigned __int128 n4 = n2 * n2;              /* n^4   */
    unsigned __int128 n5 = n4 * n128;            /* n^5   */
    unsigned __int128 X  = isqrt_uint128(n5);    /* floor(n^(5/2)) */
    unsigned __int128 X2 = X * X;                /* X^2   */
    unsigned __int128 Y  = icbrt_uint128(X2);    /* floor(X^(2/3)) */
    return (seq_t)Y;
}
