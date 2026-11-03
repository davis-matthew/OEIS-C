/*
 * A077347  Final terms of rows of A077346.
 * https://oeis.org/A077347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th square whose decimal representation starts with
 *            the digits of n
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 30, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 46 terms through n = 8277 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
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

/* ceil(sqrt(x)) using only seq_t operations */
static seq_t ceil_sqrt(seq_t x)
{
    seq_t r = floor_sqrt(x);
    if (r * r < x)
        r++;
    return r;
}

/* Find the n-th square that begins with the decimal digits of n */
static seq_t find_nth_square_prefix(idx_t n)
{
    seq_t nth = (seq_t)n;
    seq_t cum = 0;

    for (size_t m = 0;; ++m)
    {
        seq_t pow10m  = pow10((idx_t)m);
        seq_t baseLow  = (seq_t)n * pow10m;
        seq_t baseHigh = ((seq_t)n + 1) * pow10m - 1;

        seq_t low  = ceil_sqrt(baseLow);
        seq_t high = floor_sqrt(baseHigh);

        if (high >= low)
        {
            seq_t count = high - low + 1;
            if (cum + count >= nth)
            {
                seq_t offset = nth - cum - 1;
                seq_t k = low + offset;
                return k * k;
            }
            cum += count;
        }
    }
    /* Unreachable for valid inputs */
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n<1 */

    return find_nth_square_prefix(n);
}
