/*
 * A122197  Fractal sequence: count up to successive integers twice.
 * https://oeis.org/A122197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = ((n-1) mod (t+1)) + 1, where t = floor((sqrt(4*n-3)-1)/2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static idx_t isqrt_uint128(__uint128_t v)
{
    /* Integer square root of a 128‑bit value (floor). */
    idx_t low = 0;
    idx_t high = ((idx_t)1ULL << 33); /* 4*n-3 < 2^66, so sqrt < 2^33 */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __uint128_t mid2 = (__uint128_t)mid * mid;
        if (mid2 <= v)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* n >= 1; sequence is 1‑based */
    __uint128_t val = (__uint128_t)4 * n - 3;          /* 4*n-3, avoid overflow */
    idx_t s = isqrt_uint128(val);                     /* floor(sqrt(4*n-3)) */
    idx_t t = (s - 1) >> 1;                           /* floor((s-1)/2)   */
    idx_t period = t + 1;                             /* length of the cycle */
    seq_t result = ((n - 1) % period) + 1;
    return result;
}
