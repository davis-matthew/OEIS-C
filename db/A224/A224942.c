/*
 * A224942  Non-leap years beginning and ending on Thursday.
 * https://oeis.org/A224942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th non‑leap year whose 1 Jan falls on Thursday (a
 *            non‑leap year that starts on Thursday automatically ends on
 *            Thursday)
 * Range:     u8 none, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return weekday of 1 Jan for year Y: 0=Saturday, 1=Sunday, …, 5=Thursday, 6=Friday. */
static inline seq_t day_start_week(seq_t year)
{
    seq_t y = year - 1;                     /* use year-1 for January in Zeller’s formula */
    seq_t h = (seq_t)(1 + 36 + y + y / 4 - y / 100 + y / 400) % 7;
    return h;
}

seq_t a(idx_t n)
{
    seq_t year = 1587;          /* first term in the OEIS entry */
    idx_t count = 0;

    while (1) {
        bool leap = ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
        if (!leap) {
            if (day_start_week(year) == 5) {   /* Thursday */
                count++;
                if (count == n) {
                    return year;
                }
            }
        }
        year++;
    }
}
