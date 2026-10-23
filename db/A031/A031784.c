/*
 * A031784  Numbers k such that the period of the continued fraction for
 *          sqrt(k) contains exactly 16 ones.
 * https://oeis.org/A031784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer k such that the period of the *
 *            continued fraction for sqrt(k) contains exactly sixteen 1's
 * Range:     u8 none, u16 n <= 2527, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root of a 64‑bit unsigned integer.
 * Returns ⌊√n⌋. Uses binary search; avoids overflows. */
static inline uint64_t floor_sqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = (uint64_t)1ULL << 32;   /* hi² > 2⁶⁴−1 */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Return true iff the period of the continued fraction
 * for sqrt(k) contains exactly 16 occurrences of the digit 1. */
static inline bool period_has_sixteen_ones(uint64_t k)
{
    uint64_t a0 = floor_sqrt_u64(k);
    uint64_t m   = 0;
    uint64_t d   = 1;
    uint64_t a_i = a0;
    uint64_t ones = 0;

    for (;;) {
        /* m = d * a_i – m  (overflow is avoided using 128‑bit integer) */
        unsigned __int128 prod = (unsigned __int128)d * a_i;
        m = (uint64_t)(prod - m);

        /* d = (k – m²) / d */
        unsigned __int128 diff = (unsigned __int128)k - (unsigned __int128)m * m;
        d   = (uint64_t)(diff / d);

        /* a_i = ⌊(a0 + m) / d⌋  – the next partial quotient */
        a_i = (a0 + m) / d;

        if (a_i == 1)
            ++ones;
        if (ones > 16)            /* early exit – too many ones */
            return false;

        if (a_i == 2 * a0)        /* period terminates */
            break;
    }
    return ones == 16;
}

/* Main sequence function.  Sequentially scans forward until
 * the nth qualifying integer is found.  Works for any
 * seq_t defined by the build. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    uint64_t k   = 1;          /* start just before the first candidate */

    while (found < n) {
        ++k;
        uint64_t a0 = floor_sqrt_u64(k);
        if (a0 * a0 == k)           /* skip perfect squares */
            continue;
        if (period_has_sixteen_ones(k)) {
            ++found;
            if (found == n)
                return (seq_t)k;    /* cast to generic seq_t type */
        }
    }
    return (seq_t)0;               /* never reached */
}
