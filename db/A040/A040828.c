/*
 * A040828  Continued fraction for sqrt(858).
 * https://oeis.org/A040828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Validated: partial, pass, 84 terms through n = 83 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued‑fraction partial quotients for √858.
   a(0) = 29; thereafter periodic of length 4: [3, 2, 3, 58]       */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

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

seq_t a(idx_t n)
{
    /* D = 858 is a fixed non‑square integer. */
    static const idx_t D = 858;

    /* Static state for the periodic part. */
    static bool initialized = false;
    static seq_t cycle[64];
    static size_t period_len = 0;
    static seq_t a0;          /* a0 = ⌊√858⌋ as seq_t */
    static idx_t  a0i;        /* a0 as idx_t for integer arithmetic */

    /* First‑time initialization: compute the period of the continued fraction. */
    if (!initialized) {
        /* Compute a0 and its integer representation. */
        a0i = (idx_t)floor_sqrt((seq_t)D);
        a0  = (seq_t)a0i;

        /* Standard continued‑fraction algorithm for √D. */
        idx_t m = 0;
        idx_t d = 1;
        idx_t a_cur = a0i;
        size_t i = 0;

        while (1) {
            m = d * a_cur - m;
            d = (D - m * m) / d;
            a_cur = (a0i + m) / d;

            /* Store the next partial quotient. */
            cycle[i++] = (seq_t)a_cur;

            /* Period ends when a_k == 2*a0. */
            if (a_cur == 2 * a0i)
                break;
        }
        period_len = i;
        initialized = true;
    }

    /* Return the requested term. */
    if (n == 0)
        return a0;

    idx_t idx = (n - 1) % period_len;
    return cycle[idx];
}
