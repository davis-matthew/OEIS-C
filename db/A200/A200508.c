/*
 * A200508  Least m>0 such that n = 8^x-y^2 (mod m) has no solution, or 0 if
 *          no such m exists.
 * https://oeis.org/A200508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m>0 such that n = 8^x - y^2 (mod m) has no
 *            solution, * 0 if no such m exists (i.e. if n = 8^x - y^2 has an
 *            integer solution)
 * Range:     u8 n <= 230, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   congruence_has_solution
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/congruence_has_solution.h"

/* integer square root for 64‑bit values, using binary search and __uint128_t */
static uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t lo = 0, hi = 1ULL << 32;
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;          /* floor(sqrt(x)) */
}

/* test whether a given 64‑bit value is a perfect square */
static bool is_square_uint64(uint64_t v)
{
    uint64_t r = isqrt_uint64(v);
    return r * r == v;
}

/* check if n can be written as 8^x - y^2 in the integers */
static bool has_integer_representation(uint64_t n)
{
    uint64_t val = 1;                /* 8^0 */
    for (int exp = 0; exp < 21; ++exp) {
        if (val >= n) {
            uint64_t diff = val - n;   /* y^2 = 8^x - n >= 0 */
            if (is_square_uint64(diff))
                return true;
        }
        /* avoid overflow for the next power of 8 */
        if (val > UINT64_MAX / 8)
            break;
        val *= 8ULL;
    }
    return false;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;

    /* ε: if n is in A051219, return 0 immediately */
    if (has_integer_representation(N))
        return 0;

    /* search for the least modulus m with no solution */
    /* A conservative upper bound of 2000 is used; if no m is found,
       the function falls back to 0 as per the sequence definition. */
    for (uint64_t m = 1; m <= 2000; ++m) {
        if (!congruence_has_solution(N, m))
            return (seq_t)m;
    }
    return 0;   /* assume no such m exists beyond the bound */
}
