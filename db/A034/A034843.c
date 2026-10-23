/*
 * A034843  Number of divisors = first digit.
 * https://oeis.org/A034843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _first_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n), the n‑th positive integer whose
   number of positive divisors equals its first (most‑significant)
   decimal digit.  The function searches sequentially and does
   not rely on any pre‑computed tables. */

#include <stdint.h>
#include "../utils/_first_digit.h"

/* Integer square root for 64‑bit values (C++ and C). */
static uint64_t _isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = n;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        /* use 128‑bit to avoid overflow */
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Count positive divisors of a 64‑bit integer. */
static uint64_t _divisor_count_u64(uint64_t n)
{
    uint64_t count = 0;
    uint64_t r = _isqrt_u64(n);
    for (uint64_t d = 1; d <= r; ++d) {
        if (n % d == 0) {
            ++count;               /* divisor d */
            uint64_t other = n / d;
            if (other != d)
                ++count;           /* divisor n/d */
        }
    }
    return count;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    uint64_t candidate = 1;
    uint64_t found = 0;

    while (found < n) {
        /* Check the condition for the current candidate. */
        if (_first_digit(candidate) == (unsigned)_divisor_count_u64(candidate))
            ++found;
        if (found == n)
            return (seq_t)candidate;
        ++candidate;
    }

    /* Unreachable, but keeps the compiler happy. */
    return (seq_t)candidate;
}
