/*
 * A038673  Numbers that are not the difference between two positive cubes.
 * https://oeis.org/A038673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth natural number that cannot be expressed as b³‑a³
 *            for positive integers a < b. The function computes A038673 by
 *            generating all differences of cubes up to a growing bound and
 *            collecting the missing numbers
 * Range:     u8 n <= 6, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Standard integer types and helper functions  */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*  Floor cubic root for an unsigned integer (works for values up to 2⁶⁴)  */
static idx_t int_cuberoot(idx_t x)
{
    if (x == 0) return 0;
    idx_t lo = 0, hi = 1;
    /* Expand hi until hi³ > x */
    while ((__uint128_t)hi * hi * hi <= x) hi <<= 1;
    /* Binary search */
    while (lo + 1 < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if ((__uint128_t)mid * mid * mid <= x) lo = mid;
        else hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /*  Static state for memoisation  */
    static idx_t          maxX        = 0;     /* largest processed number   */
    static size_t         repr_cap    = 0;     /* capacity of repr array     */
    static uint8_t*       repr        = NULL;  /* 0/1 flag of representable numbers  */
    static seq_t*         nonrep      = NULL;  /* collected non‑representables   */
    static size_t         nonrep_cap  = 0;     /* capacity of nonrep array   */
    static size_t         nonrep_len  = 0;     /* number of entries in nonrep */
    static idx_t          lastChecked = 0;     /* last integer examined      */

    if (n == 0) return 0;  /* undefined for n == 0, return safe value */

    /*  Expand the sieve until we have at least n non‑representable terms  */
    while (nonrep_len < (size_t)n) {
        /*  Increase the upper bound   */
        idx_t newMaxX = (maxX == 0) ? 256 : maxX * 2;
        if (newMaxX <= maxX) newMaxX = maxX + 100;  /* safety offset */

        /*  Resize the repr array if needed   */
        if ((size_t)newMaxX + 1 > repr_cap) {
            size_t newCap = (size_t)newMaxX + 1;
            uint8_t* tmp  = (uint8_t*)realloc(repr, newCap);
            if (!tmp) exit(1);
            /*  Zero the whole new array  */
            memset(tmp, 0, newCap * sizeof(uint8_t));
            repr     = tmp;
            repr_cap = newCap;
        } else {
            /*  Zero only the newly needed part   */
            memset(repr + (size_t)maxX + 1, 0,
                   (newMaxX - maxX) * sizeof(uint8_t));
        }

        /*  Mark all differences b³‑a³ ≤ newMaxX as representable   */
        idx_t b_max = int_cuberoot(newMaxX + 1);  /* floor((newMaxX+1)^(1/3)) */
        if (b_max < 1) b_max = 1;
        for (idx_t b = 1; b <= b_max; ++b) {
            seq_t b3 = (seq_t)b * (seq_t)b * (seq_t)b;
            for (idx_t a = 1; a < b; ++a) {
                seq_t a3   = (seq_t)a * (seq_t)a * (seq_t)a;
                seq_t diff = b3 - a3;
                if (diff <= (seq_t)newMaxX) {
                    repr[(size_t)diff] = 1;
                }
            }
        }

        /*  Append the newly discovered non‑representable numbers   */
        for (idx_t i = lastChecked + 1; i <= newMaxX; ++i) {
            if (!repr[(size_t)i]) {
                if (nonrep_len >= nonrep_cap) {
                    size_t newCap = (nonrep_cap == 0) ? 64 : nonrep_cap * 2;
                    seq_t* tmp = (seq_t*)realloc(nonrep, newCap * sizeof(seq_t));
                    if (!tmp) exit(1);
                    nonrep     = tmp;
                    nonrep_cap = newCap;
                }
                nonrep[nonrep_len++] = (seq_t)i;
            }
        }

        maxX        = newMaxX;
        lastChecked = newMaxX;
    }

    return nonrep[n - 1];
}
