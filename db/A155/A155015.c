/*
 * A155015  Integer part of square root of n^11 = A008455(n).
 * https://oeis.org/A155015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( sqrt(n^11) ) (integer part of sqrt(n^11))
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 56, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(sqrt(x)) for an unsigned __int128 x.
   Returned value fits in uint64_t because sqrt(x) ≤ 2^64-1. */
static uint64_t sqrt128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1) << 64; /* hi = 2^64 */
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        unsigned __int128 sq = mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (uint64_t)(lo - 1);
}

seq_t a(idx_t n)
{
    /* For 64‑bit seq_t, the result fits in uint64_t only up to about
       n ≈ 3243. Beyond that n^11 does not fit in 128‑bit either
       and the returned value cannot be represented exactly.  The
       sequence is only required to match all values that fit in seq_t,
       so we clamp to 0 for larger n. */
    if (n > 3243u) {
        return (seq_t)0;
    }

    unsigned __int128 prod = 1;
    for (int i = 0; i < 11; ++i) {
        prod *= (unsigned __int128)n;
    }
    return (seq_t)sqrt128(prod);
}
