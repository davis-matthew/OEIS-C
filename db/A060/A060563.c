/*
 * A060563  First n digits after decimal point in the expansion of sqrt(n), or
 *          0 if n is a square. Leading zeros omitted.
 * https://oeis.org/A060563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = the first n digits after the decimal point of √n, or 0 if
 *            n is a perfect square (leading zeros omitted)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute the integer part of sqrt(n) */
    seq_t int_part = floor_sqrt((seq_t)n);

    /* Return 0 immediately if n is a square */
    if (int_part * int_part == (seq_t)n)
        return 0ULL;

    /* For 8‑byte or smaller seq_t use 128‑bit intermediates
       to avoid overflow when multiplying.  For larger or
       arbitrary‑precision seq_t, perform all arithmetic in
       seq_t itself. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 s    = (unsigned __int128)int_part;
        unsigned __int128 rem  = (unsigned __int128)n - s * s;
        unsigned __int128 digits = 0;

        for (idx_t i = 0; i < n; ++i) {
            rem *= 100;                     /* shift two decimal places */
            unsigned __int128 cand = s * 20; /* current cumulative value */
            int d;
            for (d = 9; d >= 0; --d) {
                unsigned __int128 prod = (cand + d) * d;
                if (prod <= rem) break;     /* choose the largest d */
            }
            digits = digits * 10 + d;        /* build the result integer */
            rem   -= (cand + d) * d;        /* subtract used portion */
            s      = s * 10 + d;            /* update cumulative value  */
        }
        return (seq_t)digits;
    } else {
        seq_t s      = int_part;
        seq_t rem    = (seq_t)n - s * s;
        seq_t digits = 0;

        for (idx_t i = 0; i < n; ++i) {
            rem = rem * 100;                 /* shift two decimal places */
            seq_t cand = s * 20;             /* current cumulative value */
            int d;
            for (d = 9; d >= 0; --d) {
                seq_t prod = (cand + d) * d;
                if (prod <= rem) break;      /* choose the largest d */
            }
            digits = digits * 10 + d;        /* build the result integer */
            rem   -= (cand + d) * d;        /* subtract used portion */
            s      = s * 10 + d;            /* update cumulative value  */
        }
        return digits;
    }
}
