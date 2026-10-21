/*
 * A095400  Largest value in trajectory when the following modified juggler
 *          map is iterated: n -> (1 - (n mod 2)) * floor(n^(3/4)) + (n mod 2)
 *          * floor(n^(4/3)); original exponents {1/2, 3/2} are replaced with
 *          {3/4, 4/3}.
 * https://oeis.org/A095400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest value in the trajectory of the modified * Juggler
 *            map with exponents {3/4, 4/3}. For x ≥ 1 the map is * f(x) = {
 *            floor(x^(3/4)) if x is even * { floor(x^(4/3)) if x is odd * The
 *            returned value a(n) is the maximum value encountered while *
 *            iterating f from n until 1
 * Range:     u8 n <= 20, u16 n <= 22, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute base^exp (0 <= exp <= 4) using unsigned __int128. */
static __uint128_t pow_uint128(__uint128_t base, unsigned exp)
{
    __uint128_t result = 1;
    while (exp--)
        result *= base;
    return result;
}

/* Compute floor(value^(1/exp)) for exp = 3 or 4.  `value` fits
 * in unsigned __int128.  The result fits in seq_t. */
static seq_t root_floor_uint128(__uint128_t value, unsigned exp)
{
    if (value <= 1)
        return (seq_t)value;

    __uint128_t low = 1, high = 1;
    /* Find an upper bound such that high^exp > value. */
    while (pow_uint128(high, exp) <= value)
        high <<= 1;

    low = high >> 1;
    while (low <= high) {
        __uint128_t mid = low + ((high - low) >> 1);
        __uint128_t power = pow_uint128(mid, exp);
        if (power <= value)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return (seq_t)high;
}

/* Compute one step of the modified Juggler map.  For even n use
 * floor(n^(3/4)), for odd n use floor(n^(4/3)). */
static seq_t juggler_next(seq_t cur)
{
    /* Interpret cur as an unsigned 64‑bit integer for bit test. */
    idx_t n = (idx_t)cur;

    if (n & 1ULL) {                 /* odd: n -> floor(n^(4/3)) */
        __uint128_t v = (__uint128_t)n * n * n * n; /* n^4 */
        return root_floor_uint128(v, 3);
    } else {                         /* even: n -> floor(n^(3/4)) */
        __uint128_t v = (__uint128_t)n * n * n;    /* n^3 */
        return root_floor_uint128(v, 4);
    }
}

/* Public interface required by the contest system. */
seq_t a(idx_t n)
{
    seq_t cur  = (seq_t)n;
    seq_t peak = cur;

    while (cur != (seq_t)1) {
        cur = juggler_next(cur);
        if (cur > peak)
            peak = cur;
    }
    return peak;
}
