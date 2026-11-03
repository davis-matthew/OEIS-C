/*
 * A095863  Index of largest triangular number with n digits.
 * https://oeis.org/A095863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = ⌊(√(8·10ⁿ – 7) – 1)/2⌋ Computes the index of the largest
 *            triangular number with n decimal digits. For 64‑bit seq_t the
 *            computation is carried out on unsigned __int128 to avoid
 *            overflow; for arbitrary‑precision seq_t it delegates to the
 *            helper routines in ../utils
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-01)
 * Helpers:   ipow_i32, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"
#include "../utils/ipow_u64_idx.h"

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

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /* 64‑bit (or smaller) implementation using __int128 for safety. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 pow10 = 1;
        for (idx_t i = 0; i < n; ++i)
            pow10 *= 10;

        unsigned __int128 D = pow10 * 8 - 7;

        /* binary search for floor(sqrt(D)) */
        uint64_t lo = 0;
        uint64_t hi = (uint64_t)1 << 32;   /* surely > sqrt(D) */

        while ((unsigned __int128)hi * hi > D && hi > 0)
            hi >>= 1;
        while ((unsigned __int128)hi * hi <= D)
            hi <<= 1;

        while (lo < hi) {
            uint64_t mid = (lo + hi + 1) >> 1;
            if ((unsigned __int128)mid * mid <= D)
                lo = mid;
            else
                hi = mid - 1;
        }
        uint64_t sqrtD = lo;

        seq_t result = (seq_t)((sqrtD - 1) >> 1);
        return result;
    }

    /* arbitrary‑precision implementation using utilities. */
    seq_t pow10_n = ipow_u64_idx(10, n);          /* 10ⁿ  */
    seq_t D = mul_small(pow10_n, 8);          /* 8·10ⁿ  */
    D -= (seq_t)7;                            /* 8·10ⁿ – 7  */
    seq_t sqrtD = floor_sqrt(D);              /* ⌊√(8·10ⁿ – 7)⌋ */
    seq_t result = (sqrtD - (seq_t)1) >> 1;   /* (sqrtD – 1)/2  */
    return result;
}
