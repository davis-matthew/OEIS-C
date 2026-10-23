/*
 * A111891  Number of numbers m <= n such that 1 equals the second digit after
 *          decimal point of square root of n in decimal representation.
 * https://oeis.org/A111891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = # { 1 ≤ m ≤ n : second decimal digit of √m equals 1 }
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: for each m, compute s = ⌊√(m·10000)⌋ and test s % 10 == 1 */

#include <stdint.h>

/* Integer square root for unsigned __int128 */
static unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 lo = 0;
    unsigned __int128 hi = (unsigned __int128)1 << 64; /* sqrt(max 128-bit) < 2^64 */

    while (lo < hi) {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t m = 1; m <= n; ++m) {
        unsigned __int128 m_scaled = (unsigned __int128)m * 10000ULL;
        unsigned __int128 s128 = isqrt128(m_scaled);
        unsigned long long digit = (unsigned long long)(s128 % 10);
        if (digit == 1)
            ++count;
    }
    return count;
}
