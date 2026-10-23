/*
 * A105642  Composite nonsquares and noncubes.
 * https://oeis.org/A105642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   composite nonsquares and noncubes. * a(n) = the n-th composite
 *            integer that is not a perfect square nor a perfect cube. * This
 *            is computed by a simple enumeration that tests each integer for:
 *            * (1) composite (not prime), * (2) not a perfect square, * (3)
 *            not a perfect cube
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   icbrt64, is_cube_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/icbrt64.h"
#include "../utils/is_cube_uint64.h"

/* --- helper functions --------------------------------------------------- */

/* integer square root (floor).  Uses binary search on 64‑bit range. */
static uint64_t isqrt64(uint64_t x)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32;  /* 2^32 > sqrt(2^64) */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > x / mid)      // mid*mid > x  (avoid overflow)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* basic trial division primality test for 64‑bit integers. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)           return false;
    if (n % 2 == 0)      return n == 2;
    if (n % 3 == 0)      return n == 3;
    uint64_t i = 5;
    while (i * i <= n) {
        if (n % i == 0)   return false;
        if (n % (i + 2) == 0) return false;
        i += 6;
    }
    return true;
}

/* composite check */
static bool is_composite_uint64(uint64_t n)
{
    return n > 1 && !is_prime_uint64(n);
}

/* perfect square check */
static bool is_square_uint64(uint64_t n)
{
    uint64_t r = isqrt64(n);
    return r * r == n;
}

/* --- sequence implementation -------------------------------------------- */

seq_t a(idx_t n)
{
    /* Indexing starts at 1.  a(1)=6. */
    idx_t count = 0;
    uint64_t candidate = 4;          /* start below first possible term */

    while (count < n) {
        candidate++;
        if (is_composite_uint64(candidate) &&
            !is_square_uint64(candidate) &&
            !is_cube_uint64(candidate)) {
            count++;
        }
    }

    return (seq_t)candidate;
}
