/*
 * A115417  Dates of Sundays starting from Jan 01 2001.
 * https://oeis.org/A115417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = day of month of the nth Sunday after 2001-01-01
 *            (Gregorian calendar, Sundays occur every 7 days starting from
 *            2001-01-01 which is a Monday)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return 1 if year y is a leap year in the Gregorian calendar. */
static int is_leap_year(uint64_t y)
{
    return (y % 4 == 0) && ((y % 100 != 0) || (y % 400 == 0));
}

/* Return number of days in year y. */
static uint64_t days_in_year(uint64_t y)
{
    return is_leap_year(y) ? 366ULL : 365ULL;
}

/* Return number of days in month m (1‑12) of year y. */
static uint64_t days_in_month(uint64_t y, int m)
{
    static const uint64_t month_len[12] = {
        31ULL, 28ULL, 31ULL, 30ULL, 31ULL, 30ULL,
        31ULL, 31ULL, 30ULL, 31ULL, 30ULL, 31ULL
    };
    if (m == 2 && is_leap_year(y))
        return 29ULL;
    return month_len[m - 1];
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* The nth Sunday lies (6 + 7*(n-1)) days after 2001-01-01. */
    uint64_t days = 6ULL + 7ULL * (uint64_t)(n - 1ULL);

    /* Find the year containing this days‑offset. */
    uint64_t year = 2001ULL;
    while (days >= days_in_year(year)) {
        days -= days_in_year(year);
        ++year;
    }

    /* Find the month and day within the year. */
    int month;
    for (month = 1; month <= 12; ++month) {
        uint64_t ml = days_in_month(year, month);
        if (days < ml) {
            /* days is zero‑based, so add 1 for the day of month. */
            return (seq_t)(days + 1ULL);
        }
        days -= ml;
    }

    /* Should never reach here. */
    return (seq_t)1ULL;
}
