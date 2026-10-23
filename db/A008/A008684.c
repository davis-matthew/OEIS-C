/*
 * A008684  Dates of successive days in Gregorian calendar.
 * https://oeis.org/A008684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, fini, full
 * Formula:   a(n) = the day of the month for day n of a non‑leap year,
 *            repeating every 365 days. The computation is a(n) = d where d =
 *            ((n-1) % 365) + 1 is the day of year, and d is reduced to the
 *            day within its month
 * Range:     u8 n <= 365, u16 n <= 365, u32 n <= 365, u64 n <= 365,
 *            u128 n <= 365, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* days in each month of a non‑leap year */
    static const idx_t month_lengths[12] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    /* day of year (1..365) within the current year cycle */
    idx_t d = ((n - 1) % 365) + 1;

    /* find month and day within that month */
    for (idx_t i = 0; i < 12; ++i) {
        if (d <= month_lengths[i]) {
            return (seq_t)d;         /* day of month */
        }
        d -= month_lengths[i];
    }

    /* Should never be reached; return 0 as a safe default. */
    return 0;
}
