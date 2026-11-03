/*
 * A062070  Smallest base b such that the decimal digits of n interpreted as
 *          digits in base b give a square, or 0 if no such base exists.
 * https://oeis.org/A062070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 100, u32 n <= 906, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 117 terms through n = 8848 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the smallest base b ≥ 2 such that interpreting the decimal
 * digits of n as digits in base b yields a perfect square.  Return that
 * base, or 0 if no such base exists.  Uses floor_sqrt to test squares.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

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

seq_t a(idx_t n)
{
    /* For n == 0 the decimal digit is “0”.  In any base it evaluates to 0,
     * which is 0².  The smallest allowed base is 2. */
    if (n == 0)
        return (seq_t)2;

    /* Extract decimal digits of n into a buffer (least‑significant first). */
    uint8_t digits[20];
    size_t len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    /* Search bases from 2 up to n + 1.  The minimal base, if it exists,
     * never exceeds n + 1.  (For n ≥ 1, base = 10 gives the value n, so
     * the search is guaranteed to find a qualifying base ≤ n + 1
     * whenever one exists.) */
    for (idx_t b = 2; b <= n + 1; ++b) {
        seq_t val = 0;
        /* Evaluate the digits in base b (most‑significant first). */
        for (int i = (int)len - 1; i >= 0; --i) {
            val = val * (seq_t)b + (seq_t)digits[i];
        }
        seq_t s = floor_sqrt(val);
        if (s * s == val)
            return (seq_t)b;
    }
    return (seq_t)0;   /* no suitable base found */
}
