/*
 * A178054  Numbers representing the index of the day of week for the first
 *          day of the month in the Gregorian calendar.
 * https://oeis.org/A178054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = day‑of‑week index (0=Sunday … 6=Saturday) for the * first
 *            day of the month number n, * relative to the Gregorian calendar.
 *            * a(1) corresponds to January 1, 2000 which is a Saturday (index
 *            6). * This implementation computes the year‑month from n and
 *            counts the * number of days elapsed since...
 * Range:     u8 n <= 9, u16 n <= 2541, u32 n <= 4800, u64 n <= 4800,
 *            u128 n <= 4800, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool is_leap(uint64_t year) {
    return (year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0));
}

seq_t a(idx_t n) {
    /* Index must be >= 1.  For n < 1 nothing is defined; we return 0. */
    if (n < 1) return 0;

    /* Convert the linear month index n into calendar year and month. */
    idx_t idx = n - 1;                      /* 0‑based offset */
    uint64_t year = 2000 + idx / 12;        /* year starting from 2000 */
    int month = (int)(idx % 12) + 1;        /* 1‑based month (1…12) */

    /* Length of months – February handled separately for leap years. */
    static const int month_len[12] = {
        31, /* Jan */
        28, /* Feb (adjusted if leap) */
        31, /* Mar */
        30, /* Apr */
        31, /* May */
        30, /* Jun */
        31, /* Jul */
        31, /* Aug */
        30, /* Sep */
        31, /* Oct */
        30, /* Nov */
        31  /* Dec */
    };

    /* Count days from 2000‑01‑01 up to the first day of the target month. */
    uint64_t days = 0;

    /* Add whole years between 2000 and year‑1. */
    for (uint64_t y = 2000; y < year; ++y) {
        days += is_leap(y) ? 366 : 365;
    }

    /* Add months within the target year that come before the target month. */
    for (int m = 1; m < month; ++m) {
        int len = month_len[m - 1];
        if (m == 2 && is_leap(year)) {
            len = 29;                       /* February in a leap year */
        }
        days += len;
    }

    /* Base day for 2000‑01‑01 is Saturday (index 6). */
    seq_t result = (seq_t)((6 + (seq_t)days) % 7);
    return result;
}
