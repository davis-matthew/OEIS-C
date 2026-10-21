/*
 * A077624  Largest term in periodic part of continued fraction expansion of
 *          square root of 2^n + 1 or 0 if 2^n + 1 is a square.
 * https://oeis.org/A077624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * floor(sqrt(2^n + 1)), unless 2^n + 1 is a perfect
 *            square, then 0
 * Range:     u8 n <= 13, u16 n <= 29, u32 n <= 61, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   floor_sqrt, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"          /* seq_t pow2(idx_t exp); */
#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t x); */

static seq_t sqrt_uint128(unsigned __int128 x)
{
    /* Integer square‑root of an unsigned 128‑bit value. */
    unsigned __int128 lo = 0, hi = ((__uint128_t)1 << 64); /* hi = 2^64 > sqrt(2^128) */
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return (seq_t)lo;
}

seq_t a(idx_t n)
{
#if defined(__SIZEOF_INT128__)
    /* 64‑bit build or smaller types */
    if (sizeof(seq_t) <= 8) {
        if ((n & 1ULL) == 0ULL) {          /* even n */
            unsigned __int128 half = ((unsigned __int128)1) << (n / 2);
            return (seq_t)(half * 2);
        } else {
            /* odd n : compute D = 2^n + 1 using 128‑bit arithmetic */
            if (n >= 128) {                /* too large for 128‑bit, not needed in 64‑bit build */
                return (seq_t)0;
            }
            unsigned __int128 D = (((unsigned __int128)1) << n) + 1;
            seq_t s = sqrt_uint128(D);
            if ((unsigned __int128)s * s == D)
                return (seq_t)0;
            return (seq_t)((unsigned __int128)s * 2);
        }
    } else
#endif
    {
        /* arbitrary‑precision build */
        seq_t D = pow2(n) + (seq_t)1;
        seq_t s = floor_sqrt(D);
        if (s * s == D)
            return (seq_t)0;
        return s * (seq_t)2;
    }
}
