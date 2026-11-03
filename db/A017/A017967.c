/*
 * A017967  Powers of sqrt(21) rounded down.
 * https://oeis.org/A017967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(21^n))
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 652 (2026-11-01)
 * Helpers:   ipow_i32, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"
#include "../utils/ipow_idx.h"

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

/* Return a(n) for n >= 0.  Works correctly for the 64‑bit build
 * for all n where the value fits in 64 bits (n <= 29) and for
 * arbitrary‑precision builds for all n. */
seq_t a(idx_t n)
{
    /* Use fast 128‑bit arithmetic when seq_t is 64‑bit.  */
    if (sizeof(seq_t) <= 8) {
        idx_t k = n / 2;            /* integer division */
        idx_t m = n % 2;            /* parity */

        __uint128_t power = 1;      /* 21^k  (k <= 14 for 64‑bit results) */
        for (idx_t i = 0; i < k; ++i)
            power *= 21;

        if (m == 0) {               /* even n: floor(21^(n/2)) = 21^k   */
            return (seq_t)power;
        }

        /* odd n: floor(sqrt(21 * (21^k)^2))  */
        __uint128_t x = (__uint128_t)21 * power * power;

        /* integer square root of x (x <= 21 * (21^14)^2 < 2^128) */
        __uint128_t low  = 0;
        __uint128_t high = (__uint128_t)1 << 64;  /* > sqrt(2^128) */
        while (low < high) {
            __uint128_t mid = (low + high + 1) >> 1;
            if (mid * mid <= x)
                low = mid;
            else
                high = mid - 1;
        }
        return (seq_t)low;          /* result fits in 64 bits */
    }

    /* arbitrary‑precision build: use big arithmetic helpers */
    seq_t p = ipow_idx((seq_t)21, n);
    return floor_sqrt(p);
}
