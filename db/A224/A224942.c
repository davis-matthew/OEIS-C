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
 * Helpers:   day_start_week
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/day_start_week.h"

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
