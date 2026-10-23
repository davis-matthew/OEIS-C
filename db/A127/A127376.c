/*
 * A127376  Perpetual calendar, giving the calendar for successive years of
 *          the 400-year cycle starting in year xy01, where xy is a 2-digit
 *          multiple of 4. Calendars 1 to 7 are for normal years starting on
 *          Monday, Tuesday, ..., Sunday; 8 to 14 are for leap years likewise.
 * https://oeis.org/A127376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 400, u16 n <= 400, u32 n <= 400, u64 n <= 400,
 *            u128 n <= 400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A127376: perpetual calendar codes for a 400‑year Gregorian cycle.
 * a(n) is the code of the year 2000 + ((n-1) mod 400) + 1.
 * Normal years (non‑leap) are numbered 1..7 for Monday..Sunday.
 * Leap years are numbered 8..14 for Monday..Sunday.
 * The table for the first 400 years is built on first call.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool ready = false;
    static seq_t table[400];

    if (!ready) {
        /* Build the 400‑year cycle starting at 2001. */
        seq_t weekday = 1; /* Monday */
        for (idx_t i = 0; i < 400; ++i) {
            idx_t year = 2001 + i;
            bool isLeap = (year % 400 == 0) ||
                          (year % 4 == 0 && year % 100 != 0);
            table[i] = weekday + (isLeap ? 7 : 0);

            /* advance to next year's Jan‑1 weekday */
            weekday += (isLeap ? 2 : 1);
            if (weekday > 7) weekday -= 7;
        }
        ready = true;
    }

    idx_t m = (n - 1) % 400;
    return table[m];
}
