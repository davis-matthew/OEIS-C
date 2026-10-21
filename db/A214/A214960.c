/*
 * A214960  Expansion of psi(x^2) - x * psi(x^10) in powers of x where psi()
 *          is a Ramanujan theta function.
 * https://oeis.org/A214960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 if n = k(k-1) for some k≥1, * -1 if n = 5*k(k-1)+1 for
 *            some k≥1, * 0 otherwise
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function tests whether a 64‑bit integer x is of the form
 *  k(k-1) by checking if 4*x+1 is an odd perfect square.
 *  Integer square roots for 128‑bit values are computed by a
 *  binary‑search routine that uses only 64‑bit arithmetic
 *  and 128‑bit multiplication for the comparison. */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for a 128‑bit unsigned value.
 * Returns floor( sqrt(x) ). */
static inline uint64_t isqrt_u128(__uint128_t x)
{
    uint64_t low = 0, high = (uint64_t)~0ULL;      /* 2^64-1 */
    while (low < high) {
        uint64_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == x) return mid;
        if (sq < x)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

/*  Returns true iff x = k(k-1) for some integer k. */
static inline bool is_triangular(idx_t x)
{
    __uint128_t val = (__uint128_t)4 * x + 1;  /* 4x+1 is always odd if x is triangular */
    uint64_t s = isqrt_u128(val);
    return (__uint128_t)s * s == val;
}

seq_t a(idx_t n)
{
    /* First check the +1 term */
    if (is_triangular(n))
        return (seq_t)1;

    /* Then check the -1 term */
    if (n >= 1 && (n - 1) % 5 == 0) {
        idx_t m = (n - 1) / 5;
        if (is_triangular(m))
            return (seq_t)-1;
    }
    return (seq_t)0;
}
