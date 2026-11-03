/*
 * A224950  Leap years having 53 Fridays and Saturdays.
 * https://oeis.org/A224950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: iterate over years starting at 1583 and count leap years
 * whose January 1 is a Friday.  The Gregorian calendar repeats every
 * 400 years, so this produces the OEIS sequence A224950 for all
 * n ≥ 1. */

#include <stdint.h>
#include <stdbool.h>

/* Return weekday of January 1 of year y:
 * 0 = Sunday, 1 = Monday, …, 5 = Friday, 6 = Saturday.
 * Implementation uses Zeller’s congruence. */
static inline int jan1_weekday(uint64_t y)
{
    uint64_t Y = y - 1ULL;                 /* month ≤ 2 → year = y‑1 */
    uint64_t K = Y % 100ULL;               /* year within century   */
    uint64_t J = Y / 100ULL;               /* zero‑based century    */

    /* Zeller formula (h = 0 = Saturday, 1 = Sunday, …, 6 = Friday). */
    uint64_t h = (1ULL + 36ULL + K + K / 4ULL + J / 4ULL + 5ULL * J) % 7ULL;
    /* Convert to 0 = Sunday … 6 = Saturday. */
    return (int)(((int)h + 6) % 7);
}

static inline bool is_leap_year(uint64_t y)
{
    return ((y % 4ULL == 0ULL) && ((y % 100ULL != 0ULL) || (y % 400ULL == 0ULL)));
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t count  = 0;
    /* The first year satisfying the property is 1588.  Start
       our search after the Gregorian reform. */
    for (uint64_t year = 1583ULL; count < n; ++year) {
        if (!is_leap_year(year))
            continue;
        /* Friday is encoded as 5, see jan1_weekday(). */
        if (jan1_weekday(year) == 5) {
            ++count;
            result = (seq_t)year;
        }
    }
    return result;
}
