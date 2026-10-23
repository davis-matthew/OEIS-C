/*
 * A174431  Day of the week occurring 5 times in February, or 0 if no such day
 *          exists (1 to 7 are Sunday to Saturday).
 * https://oeis.org/A174431
 *
 * Offset:    1753    (a(1753) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is not a leap year. Otherwise a(n) is the weekday
 *            of 29 February in year n, with Sunday = 1, Monday = 2, …,
 *            Saturday = 7
 * Range:     u8 n <= 1840, u16 n <= 1840, u32 n <= 1840, u64 n <= 1840,
 *            u128 n <= 1840, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Leap‑year test for the Gregorian calendar. */
    bool is_leap = (n % 4 == 0) &&
                   ((n % 100 != 0) || (n % 400 == 0));
    if (!is_leap)
        return (seq_t)0;

    /* Zeller's congruence (Gregorian) for the date 29‑Feb‑n. */
    idx_t year = n;
    int month = 2;          /* February */
    int day   = 29;         /* 29th day */

    /* Months January and February are treated as month 13/14
       of the previous year in Zeller's formula. */
    if (month <= 2) {
        month += 12;        /* month becomes 14 */
        year--;             /* previous year */
    }

    /* Compute the components of Zeller's formula. */
    idx_t K = year % 100;                  /* year of the century */
    idx_t J = year / 100;                  /* zero‑based century */

    idx_t h = (day
               + (13 * (month + 1)) / 5
               + K
               + K / 4
               + J / 4
               + 5 * J) % 7;                /* h=0 => Saturday, 1 => Sunday, … */

    /* Convert to 0 = Sunday, 6 = Saturday. */
    idx_t dow = (h + 6) % 7;

    /* Return 1–7 (Sunday = 1). */
    return (seq_t)(dow + 1);
}
