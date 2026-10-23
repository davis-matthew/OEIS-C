/*
 * A160278  Angle in degrees between the two hands of a 12-hour analog clock
 *          at 12*n minutes after noon/midnight.
 * https://oeis.org/A160278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 * min(|(n mod 60) - (12*(n mod 5))|, 60 - |(n mod 60) -
 *            (12*(n mod 5))|)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* modulo the index to get the hour and minute marks */
    idx_t hour_mod = n % 60;       /* hour hand mark (0..59) */
    idx_t minute_step = n % 5;     /* minute hand changes every 12 minutes */

    seq_t hour   = (seq_t)hour_mod;
    seq_t minute = (seq_t)(12 * minute_step);   /* 12 * (n mod 5)   */

    /* absolute difference between the marks */
    seq_t diff = hour > minute ? hour - minute : minute - hour;

    /* use the smaller of the two possible angles on the circle */
    if (diff > (seq_t)30u)
        diff = 60 - diff;

    /* each mark represents 6 degrees */
    return diff * 6;
}
