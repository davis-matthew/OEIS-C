/*
 * A210883  Number of days after Jan 01 1000 such that the date written in the
 *          ISO-8601-format YYYYMMDD is palindromic.
 * https://oeis.org/A210883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 22, u32 n <= 331, u64 n <= 331, u128 n <= 331,
 *            big any n
 * Helpers:   days_since_epoch
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A210883: number of days after Jan 01 1000 for dates
   that are palindromic in the ISO‑8601 format YYYYMMDD.
   The function builds a table of all such dates (331 entries) and
   returns the offset for a given 1‑based index n. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/days_since_epoch.h"

static inline bool is_leap_year(int y)
{
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

static const unsigned days_in_month_norm[13] = {
    0, 31, 28, 31, 30, 31, 30,
    31, 31, 30, 31, 30, 31
};

static inline bool valid_day(int year, unsigned month, unsigned day)
{
    if (month < 1 || month > 12) return false;
    unsigned dim = days_in_month_norm[month];
    if (month == 2 && is_leap_year(year)) dim = 29;
    return day >= 1 && day <= dim;
}

seq_t a(idx_t n)
{
    /* Static cache of all palindromic dates.  The list contains
       the number of days after Jan 01 1000 for each palindromic
       YYYYMMDD date in ascending order. */
    static seq_t *pal_offsets = NULL;
    static size_t pal_count = 0;
    static size_t pal_cap = 0;

    if (pal_offsets == NULL) {
        const size_t init_cap = 4000;
        pal_cap = init_cap;
        pal_offsets = (seq_t *)malloc(pal_cap * sizeof(seq_t));
        pal_count = 0;

        const int64_t ref_days = days_since_epoch(1000, 1, 1);

        for (int year = 1000; year <= 9999; ++year) {
            int d1 = year / 1000;
            int d2 = (year / 100) % 10;
            int d3 = (year / 10) % 10;
            int d4 = year % 10;

            unsigned month = (unsigned)(d4 * 10 + d3);
            unsigned day   = (unsigned)(d2 * 10 + d1);

            if (!valid_day(year, month, day))
                continue;

            int64_t days = days_since_epoch(year, month, day);
            seq_t offset = (seq_t)(days - ref_days);

            if (pal_count >= pal_cap) {
                pal_cap *= 2;
                pal_offsets = (seq_t *)realloc(pal_offsets, pal_cap * sizeof(seq_t));
            }
            pal_offsets[pal_count++] = offset;
        }
    }

    if (n == 0 || n > pal_count)
        return 0; /* undefined for out‑of‑range indices */

    return pal_offsets[n - 1];
}
