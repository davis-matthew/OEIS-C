/*
 * A034117  Fractional part of cube root of a(n) starts with digit 1.
 * https://oeis.org/A034117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, look
 * Formula:   a(n) = integer in the interval [ (m+0.1)^3 , (m+0.2)^3 ) * where
 *            m = floor( the integer part of cube root of a(n) ), * and the
 *            fractional part of cube root starts with digit 1. * In integer
 *            arithmetic this interval is * [ ceil( (10m+1)^3 /1000 ) , floor(
 *            (10m+2)^3 /1000 ) ]. * The sequence is obt...
 * Range:     u8 n <= 30, u16 n <= 6717, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ceil_div1000
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ceil_div1000.h"

/* Assume seq_t and idx_t are defined elsewhere by the build system. */

seq_t a(idx_t n)
{
    /* 1‑based indexing: a(1) is the first term (10). */
    if (n == 0) return 0;   /* undefined, but safe */

    idx_t m = 1;            /* candidate m (the integer part of cube root) */
    seq_t cumulative = 0;   /* number of terms already counted below current m */

    while (true)
    {
        /* ----- compute lower bound ----------------------------------- */
        {
            seq_t t = (seq_t)10 * (seq_t)m + 1;          /* 10m+1  */
            unsigned __int128 t128 = (unsigned __int128)t;
            unsigned __int128 cube128 = t128 * t128 * t128;
            seq_t lower = (seq_t)((cube128 + 999) / 1000);   /* ceil */
            /* store lower for later use */
            t = lower;  /* reuse variable t */
        }

        /* ----- compute upper bound ----------------------------------- */
        seq_t upper;
        {
            seq_t t = (seq_t)10 * (seq_t)m + 2;          /* 10m+2  */
            unsigned __int128 t128 = (unsigned __int128)t;
            unsigned __int128 cube128 = t128 * t128 * t128;
            upper = (seq_t)(cube128 / 1000);  /* floor */
        }

        /* ----- compute count for this m ------------------------------ */
        seq_t lower;
        {
            seq_t t = (seq_t)10 * (seq_t)m + 1;
            unsigned __int128 t128 = (unsigned __int128)t;
            unsigned __int128 cube128 = t128 * t128 * t128;
            lower = (seq_t)((cube128 + 999) / 1000);
        }

        seq_t count = upper - lower + 1;
        if (cumulative + count >= n) /* desired term lies in this block */
        {
            seq_t offset = (seq_t)(n - cumulative - 1);
            return lower + offset;
        }

        cumulative += count;
        ++m;
    }
}
