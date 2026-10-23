/*
 * A153843  First Sunday in n-th month of 365-day year starting on Thursday.
 * https://oeis.org/A153843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the first Sunday in the n‑th month of a 365‑day year
 * starting on Thursday.  The month lengths are the standard
 * non‑leap Gregorian values.  The weekday is encoded 0=Mon … 6=Sun.
 *
 * a(1) = 4   (January 1 Thursday, first Sunday on day 4)
 * a(2) = 1   (February 1 Sunday, first Sunday on day 1)
 * ...
 */

#include <stddef.h>      /* for size_t */
#include <stdint.h>     /* for uint64_t if needed, may be unused */

static const int monthlen[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

seq_t a(idx_t n)
{
    /* n >= 1;  compute month index modulo 12 */
    size_t idx = (size_t)((n - 1) % 12);

    /* cumulative days before this month mod 7 */
    int offset = 0;
    for (size_t i = 0; i < idx; ++i)
        offset += monthlen[i];
    offset %= 7;

    /* weekday of the first day of this month:
     * 0 = Monday, ..., 6 = Sunday
     * Start of year is Thursday (3) */
    int start = (3 + offset) % 7;

    /* day of the first Sunday in this month */
    int firstSunday = ((6 - start) % 7) + 1;

    return (seq_t)firstSunday;
}
