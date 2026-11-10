/*
 * A077349  a(n) = A077347(n)^(1/2).
 * https://oeis.org/A077349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = sqrt(A077347(n)), where A077347(n) is the n-th square
 *            whose decimal representation starts with the digits of n (found
 *            by counting squares in each digit-length range)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 50, u64 n <= 500, u128 n <= 500,
 *            big any n
 * Validated: standard, pass, 66 terms through n = 454 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static seq_t A077349_pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

static seq_t A077349_floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (hi * hi <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static seq_t A077349_ceil_sqrt(seq_t x)
{
    seq_t r = A077349_floor_sqrt(x);
    if (r * r < x)
        r++;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t nth = (seq_t)n;
    seq_t cum = 0;

    for (size_t m = 0;; ++m)
    {
        seq_t p = A077349_pow10((idx_t)m);
        seq_t baseLow  = (seq_t)n * p;
        seq_t baseHigh = ((seq_t)n + 1) * p - 1;

        seq_t low  = A077349_ceil_sqrt(baseLow);
        seq_t high = A077349_floor_sqrt(baseHigh);

        if (high >= low)
        {
            seq_t count = high - low + 1;
            if (cum + count >= nth)
            {
                seq_t offset = nth - cum - 1;
                return low + offset;
            }
            cum += count;
        }
    }
    return 0;
}
