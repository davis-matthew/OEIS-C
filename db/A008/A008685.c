/*
 * A008685  Lengths of months in the Gregorian calendar.
 * https://oeis.org/A008685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/*  A008685 – Lengths of months in the Gregorian calendar.
 *  The sequence repeats every 400 years (4800 months).  For a given n
 *  (1‑based), the month is (n−1) mod 12 and the year is
 *  ((n−1)/12)+1.  February has 29 days in a leap year (year divisible
 *  by 400, or divisible by 4 but not by 100); otherwise it has 28 days.
 *  All other months have their fixed lengths. */

static const seq_t MONTH_DAYS[12] = {
    31, /* Jan */
    28, /* Feb – may become 29 */
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

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; n == 0 is outside the
     * offset and is returned as 0, which is harmless for callers. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Zero‑based month index (0 = January, 1 = February, …, 11 = December) */
    idx_t month_idx = (n - 1) % 12U;
    seq_t days = MONTH_DAYS[month_idx];

    /* Only February needs special handling. */
    if (month_idx == 1U) {
        /* Year number starting at AD 1. */
        idx_t year = ((n - 1) / 12U) + 1U;
        idx_t year_mod_400 = year % 400U;

        /* Gregorian leap‑year rule. */
        if (year_mod_400 == 0U ||
            (year_mod_400 % 4U == 0U && year_mod_400 % 100U != 0U)) {
            days = 29U;
        } else {
            days = 28U;
        }
    }

    return days;
}
