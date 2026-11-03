/*
 * A017923  Powers of sqrt(6) rounded to nearest integer.
 * https://oeis.org/A017923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round((sqrt(6))^n)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 49, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 1998 (2026-11-01)
 * Helpers:   ipow_i32, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_i32.h"
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

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

/* integer square root of an unsigned 128‑bit value, result fits in 64 bits */
static inline seq_t sqrt128(unsigned __int128 x)
{
    unsigned __int128 low = 0;
    unsigned __int128 high = ((unsigned __int128)1 << 64) - 1;
    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq == x) return (seq_t)mid;
        if (sq < x) low = mid + 1;
        else if (mid == 0) break;
        else high = mid - 1;
    }
    return (seq_t)(low - 1);
}

seq_t a(idx_t n)
{
    /* The even terms are exact powers of 6. */
    if ((n & 1ULL) == 0) {            /* even n */
        idx_t k = n >> 1;
        /* Small integer implementation for 64‑bit seq_t: */
        if (sizeof(seq_t) <= 8) {
            unsigned __int128 pow = 1;
            for (idx_t i = 0; i < k; ++i) pow *= 6;
            return (seq_t)pow;
        }
        /* Generic big‑integer implementation: */
        return ipow_idx((seq_t)6, k);
    }

    /* Odd index: a(2k+1) = round( sqrt(6) * 6^k ) */
    idx_t k = n >> 1;
    if (sizeof(seq_t) <= 8) {                   /* 64‑bit seq_t */
        unsigned __int128 pow = 1;
        for (idx_t i = 0; i < k; ++i) pow *= 6;
        uint64_t N = (uint64_t)pow;              /* 6^k fits in 64 bits here */
        unsigned __int128 val = (unsigned __int128)6 * N * N;
        seq_t r = (seq_t)sqrt128(val);           /* floor sqrt(6*N^2)  */
        unsigned __int128 r128 = (unsigned __int128)r;
        unsigned __int128 diff1 = val - r128 * r128;
        unsigned __int128 diff2 = (r128 + 1) * (r128 + 1) - val;
        return diff1 <= diff2 ? r : r + (seq_t)1;
    }

    /* Generic big‑integer implementation for arbitrary‑precision seq_t */
    seq_t N  = ipow_idx((seq_t)6, k);            /* 6^k  */
    seq_t val = seq_mul((seq_t)6, seq_mul(N, N));  /* 6*N^2  */
    seq_t r = floor_sqrt(val);                  /* floor sqrt(6*N^2)  */
    seq_t diff1 = val - seq_mul(r, r);
    seq_t diff2 = seq_mul(r + (seq_t)1, r + (seq_t)1) - val;
    return diff1 <= diff2 ? r : r + (seq_t)1;
}
