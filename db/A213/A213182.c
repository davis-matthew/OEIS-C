/*
 * A213182  Numbers which may represent a date in "condensed European
 *          notation" DDMMYY.
 * https://oeis.org/A213182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumeration: all numbers that can represent a valid Gregorian date DDMMYY
 * for years 2000‑2099.  The sequence is sorted in increasing numeric order. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static const int daysInMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

static inline bool is_leap_year(int year)
{
    return (year % 4 == 0) && ((year % 100) != 0 || (year % 400) == 0);
}

static inline int days_in_month(int month, int year)
{
    if (month == 2 && is_leap_year(year))
        return 29;
    return daysInMonth[month - 1];
}

seq_t a(idx_t n)
{
    /* The sequence contains 36525 terms – the dates 01‑01‑2000 … 31‑12‑2099. */
    static idx_t total = 36525;
    static seq_t *values = NULL;
    static bool initialized = false;

    if (!initialized) {
        values = (seq_t *)malloc(total * sizeof(seq_t));
        if (!values) return (seq_t)0;        /* allocation failed */
        idx_t pos = 0;
        for (int day = 1; day <= 31; ++day) {
            for (int month = 1; month <= 12; ++month) {
                for (int yy = 0; yy < 100; ++yy) {
                    int year = 2000 + yy;
                    if (day <= days_in_month(month, year)) {
                        values[pos++] = (seq_t)day * (seq_t)10000 +
                                        (seq_t)month * (seq_t)100 +
                                         (seq_t)yy;
                    }
                }
            }
        }
        /* pos should equal total (36525).  No check needed for speed. */
        initialized = true;
    }

    if (n == 0 || n > total) return (seq_t)0;  /* out of range returns 0 */
    return values[n - 1];
}
