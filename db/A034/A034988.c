/*
 * A034988  a(n) is root of smallest square starting with a string of n
 *          'digit_6's.
 * https://oeis.org/A034988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   floor_sqrt, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the root of the smallest square whose decimal
   representation starts with n consecutive sixes.
   For small seq_t (≤ 8 bytes) the computation is performed with
   unsigned __int128 arithmetic to avoid overflow.
   For larger seq_t (arbitrary‑precision types) normal arithmetic is
   used.  The function works for all n ≥ 1 that fit in seq_t.   */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t); */
#include "../utils/seq_pow.h"      /* seq_t seq_pow(seq_t base, idx_t exp); */

static unsigned __int128 floor_sqrt128(unsigned __int128 x)
{
    unsigned __int128 low  = 0;
    unsigned __int128 high = ((unsigned __int128)1 << 64);
    while (low < high) {
        unsigned __int128 mid = (low + high + 1) >> 1;
        if (mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* --- Small seq_t: use 128‑bit arithmetic to avoid overflow --- */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        unsigned __int128 p   = 0;
        unsigned __int128 p1  = 0;  /* p + 1 later */

        for (idx_t i = 0; i < n; ++i)
            p = p * 10 + 6;

        p1 = p + 1;

        idx_t t = 0;
        for (;;) {
            unsigned __int128 pow10t = 1;
            for (idx_t i = 0; i < t; ++i)
                pow10t *= 10;

            unsigned __int128 lower = p * pow10t;
            unsigned __int128 upper = (p1 * pow10t) - 1;

            unsigned __int128 cand = floor_sqrt128(lower);
            if (cand * cand < lower)
                ++cand;

            if (cand * cand <= upper)
                return (seq_t)cand;
            ++t;
        }
    }

    /* --- Large (arbitrary–precision) seq_t: normal arithmetic --- */
    {
        seq_t p   = 0;
        seq_t p1  = 0;  /* p + 1 later */

        for (idx_t i = 0; i < n; ++i)
            p = p * 10 + 6;

        p1 = p + 1;

        idx_t t = 0;
        for (;;) {
            seq_t pow10t = seq_pow((seq_t)10, t);
            seq_t lower  = p * pow10t;
            seq_t upper  = p1 * pow10t;
            upper = upper - (seq_t)1;

            seq_t cand = floor_sqrt(lower);
            if (cand * cand < lower)
                ++cand;

            if (cand * cand <= upper)
                return cand;
            ++t;
        }
    }
}
