/*
 * A160272  Angle between the two hands of a 12 hour analog clock n*12 minutes
 *          after noon/midnight, measured in units of minutes.
 * https://oeis.org/A160272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min(|(n mod 60) - ((12*n) mod 60)|, 60 - |(n mod 60) -
 *            ((12*n) mod 60)|)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smaller angle (in minute‑mark units) between the hour and
   minute hands of a 12‑hour analog clock n*12 minutes after noon/midnight.
   The hour hand moves one minute mark every 12 minutes, and the minute
   hand moves twelve minute marks every 12 minutes (i.e., each minute). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Hour hand position in 0..59 minute marks */
    idx_t hour_idx = n % 60;

    /* Minute hand position depends only on n mod 5 because 12*n mod 60
       repeats every 5 steps. */
    idx_t minute_idx = (12 * (n % 5)) % 60;

    seq_t hour   = (seq_t)hour_idx;
    seq_t minute = (seq_t)minute_idx;

    seq_t diff = hour >= minute ? hour - minute : minute - hour;

    if (diff <= 30)
        return diff;
    else
        return (seq_t)(60 - diff);
}
