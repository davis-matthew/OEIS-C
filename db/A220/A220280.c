/*
 * A220280  The reluctant sequence for A002260.
 * https://oeis.org/A220280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = A002260(A002260(n)) A002260(n) = largest r such that
 *            1+2+...+r ≥ n, with r = n - (t-1)*t/2 where t = ceil(
 *            (sqrt(8*n+1)-1)/2 ) Implemented using only integer arithmetic
 *            (no floating point)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_uint128(unsigned __int128 x)
{
    if (x == 0) return 0;
    idx_t low = 0, high = 1;
    while ((unsigned __int128)high * high <= x) {
        high <<= 1;
        if (high == 0) { high = UINT64_MAX; break; } /* overflow guard */
    }
    low = high >> 1;
    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((unsigned __int128)mid * mid <= x)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* Computes A002260(n) */
static inline seq_t f(idx_t n)
{
    /* t is the minimal integer with t*(t+1)/2 >= n */
    unsigned __int128 val = (unsigned __int128)8 * n + 1;
    idx_t k  = isqrt_uint128(val);
    idx_t t  = (k - 1) >> 1;   /* floor((sqrt(8*n+1)-1)/2) */
    if ((unsigned __int128)t * (t + 1) / 2 < n) t += 1;
    /* previous triangular sum (t-1)*t/2 */
    unsigned __int128 prev = (unsigned __int128)(t - 1) * t / 2;
    return (seq_t)(n - prev);
}

seq_t a(idx_t n)
{
    seq_t inner = f(n);
    return f((idx_t)inner);
}
