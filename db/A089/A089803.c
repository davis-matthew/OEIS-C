/*
 * A089803  Expansion of Jacobi theta function theta_4(q^5).
 * https://oeis.org/A089803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 For n > 0: if n % 5 != 0 -> a(n) = 0 else let m = n / 5
 *            if m is not a perfect square -> a(n) = 0 else let r = sqrt(m)
 *            a(n) = 2 * (-1)^r // 2 when r is even, -2 when r is odd
 * Range:     i8 n <= 5000, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   // for uintptr_t, uint64_t
#include <limits.h>   // for UL

/* integer square root using binary search (returns floor(sqrt(x))) */
static unsigned long long isqrt_uint64(unsigned long long x)
{
    unsigned long long low = 0;
    unsigned long long high = ((unsigned long long)1 << 32); /* sqrt(2^64-1) <= 2^32-1 */

    while (low < high) {
        unsigned long long mid = (low + high) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq <= (__int128)x)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* only multiples of 5 can appear */
    if ((n % 5) != 0)
        return 0;

    /* n = 5 * m */
    idx_t m = n / 5;

    /* check if m is a perfect square */
    unsigned long long r = isqrt_uint64((unsigned long long)m);
    if (((__int128)r * r) != (__int128)m)
        return 0;

    /* a(n) = ±2 depending on parity of r */
    return (seq_t)((r & 1) ? -2 : 2);
}
