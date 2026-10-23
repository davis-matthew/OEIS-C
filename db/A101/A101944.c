/*
 * A101944  Number of months in a 400-year cycle that have the thirteenth day
 *          of the month fall on the n-th day of the week, beginning with
 *          Sunday.
 * https://oeis.org/A101944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, dumb
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A101944
   Number of months in a 400‑year Gregorian cycle whose 13th day
   falls on weekday n (Sunday = 1, Monday = 2, …, Saturday = 7). */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The distribution of weekdays for the 13th in a 400‑year cycle
       is fixed.  We compute it once and cache the result. */
    static bool initialized = false;
    static seq_t counts[7] = {0};  /* indices: 0->Sunday, 6->Saturday */

    if (!initialized) {
        static const int month_days[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
        int start_weekday = 6;   /* 1 Jan 2000 is Saturday (internal 0=Sun..) */
        for (idx_t y = 0; y < 400; ++y) {            /* 400‑year cycle */
            idx_t year = 2000 + y;
            bool leap = ((year % 400 == 0) ||
                         ((year % 4 == 0) && (year % 100 != 0)));
            for (idx_t m = 0; m < 12; ++m) {
                int days = month_days[m];
                if (m == 1 && leap) days = 29;          /* February */
                int weekday13 = (start_weekday + 12) % 7;
                counts[weekday13] += 1;
                start_weekday = (start_weekday + days) % 7; /* next month */
            }
        }
        initialized = true;
    }

    idx_t idx = (n - 1) % 7;  /* zero‑based weekday */
    return counts[idx];
}
