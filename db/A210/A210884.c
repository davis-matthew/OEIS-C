/*
 * A210884  Dates after Jan 01 1000 which are palindromic when they are
 *          written according to the ISO-8601-format YYYYMMDD.
 * https://oeis.org/A210884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 331, u64 n <= 331, u128 n <= 331,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate palindromic dates in YYYYMMDD format for years
   1000 – 9999 by mapping the four digits of the year to month and day
   positions.  The calendar test uses the Gregorian rules for leap years.
   The first date is 10011001 (Oct 1 1001).  The generated list
   contains 331 values, and this function returns the n‑th value (1‑based). */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Estimating capacity 400 is enough for the 331 palindromic dates. */
    static seq_t dates[400];
    static size_t count = 0;
    static bool initialized = false;

    if (!initialized)
    {
        for (int year = 1000; year <= 9999; ++year)
        {
            /* Extract year digits */
            int y1 = (year / 1000) % 10;          /* Y1 */
            int y2 = (year / 100) % 10;           /* Y2 */
            int y3 = (year / 10) % 10;            /* Y3 */
            int y4 = year % 10;                   /* Y4 */

            /* Month = Y4Y3, Day = Y2Y1 */
            int month = 10 * y4 + y3;             /* M1M2 */
            int day   = 10 * y2 + y1;             /* D1D2 */

            if (month < 1 || month > 12)
                continue;                        /* invalid month */

            /* Number of days in month, Gregorian rules */
            int max_day;
            switch (month)
            {
                case 2:
                    /* Leap year check: divisible by 400 or by 4 but not 100 */
                    if ((year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0)))
                        max_day = 29;
                    else
                        max_day = 28;
                    break;
                case 4: case 6: case 9: case 11:
                    max_day = 30;
                    break;
                default:
                    max_day = 31;
                    break;
            }

            if (day < 1 || day > max_day)
                continue;                        /* invalid day */

            /* Pack into YYYYMMDD numeric value */
            dates[count++] = (seq_t)year * (seq_t)10000 +
                              (seq_t)month * (seq_t)100 +
                               (seq_t)day;
        }
        initialized = true;
        /* The list is sorted by year because we iterated in ascending order. */
    }

    if (n == 0 || n > count)
        return 0;               /* out of range – undefined for the sequence */

    return dates[n - 1];
}
