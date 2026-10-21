/*
 * A073423  Sums of two powers of zero: triangle read by rows: T(m,n) = 0^n +
 *          0^m, n >= 0, m = 0..n.
 * https://oeis.org/A073423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(0) = 2; for n > 0, a(n) = 1 if n is a triangular number,
 *            otherwise 0. (Triangular numbers are those n where there exists
 *            k with n = k*(k+1)/2.)
 * Range:     u8 n <= 9937, u16 n <= 9937, u32 n <= 9937, u64 n <= 9937,
 *            u128 n <= 9937, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the integer square root of an unsigned __int128 value.
   The result fits in 64 bits because the input never exceeds 2^67-1. */
static uint64_t isqrt_u128(__int128 v)
{
    uint64_t low = 0;
    /* 2^34 > sqrt(8*2^64)  and also > sqrt(2^67-1) */
    uint64_t high = (uint64_t)1 << 34;

    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;

        if (sq == v) {
            return mid;
        } else if (sq < v) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;   /* floor of the square root */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;

    /* Compute u = 1 + 8*n, which fits in __int128. */
    __int128 u = (__int128)1 + (__int128)8 * n;
    uint64_t sqrt_u = isqrt_u128(u);

    /* Check whether u is a perfect square. */
    if ((__int128)sqrt_u * sqrt_u == u && (sqrt_u & 1ULL)) {
        return (seq_t)1;  /* n is triangular */
    }
    return (seq_t)0;      /* n is not triangular */
}
