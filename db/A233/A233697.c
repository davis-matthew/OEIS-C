/*
 * A233697  Duration in months between successive instances of a given Julian
 *          calendar day of the month falling on a specified day of the week.
 * https://oeis.org/A233697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force computation of successive month gaps for a
   fixed Julian‑calendar day-of‑month (here the 17th) falling on a
   particular weekday (the weekday it has in the first year).  The gaps
   repeat every 28 years (48 gaps).  This implements OEIS A233697. */

#include <stddef.h>
#include <stdbool.h>

enum { PERIOD = 48 };          /* 48 gaps in the 28‑year cycle */

/* auxiliary table: days in each month (non‑leap) */
static const int mdays[12] = { 31, 28, 31, 30, 31, 30,
                               31, 31, 30, 31, 30, 31 };

/* true for month 0 (January of the starting leap year), which is
   guaranteed to contain the target day on the target weekday */
static bool month_has_event(idx_t year, int month, int target_day,
                            int tgt_weekday, int *weekday_of_first);

/* number of days in a month of a Julian year */
static inline int days_in_month(idx_t year, int month) {
    if (month == 1)                 /* February */
        return (year % 4 == 0) ? 29 : 28;
    return mdays[month];
}

/* Compute the weekday of the first day of the given month.  The
   weekday numbering is arbitrary; 0 is an arbitrary weekday.  The
   function also returns the weekday of the target_day within the
   month. */
static inline void month_weekdays(idx_t year, int month,
                                  int target_day, int *first_weekday,
                                  int *weekday_of_target) {
    static int global_first_weekday = 0;        /* Jan 1 weekday, 0‑based */
    *first_weekday = global_first_weekday;
    *weekday_of_target = (*first_weekday + (target_day - 1)) % 7;
    /* advance the global weekday for next month */
    global_first_weekday = (*first_weekday + days_in_month(year, month)) % 7;
}

/* build the 48‑gap cycle once;   cycle array holds the gaps
   of successive occurrences of the target day on the target weekday */
static seq_t cycle[PERIOD];
static bool cycle_built = false;

static void build_cycle(void) {
    if (cycle_built) return;

    const int TARGET_DAY = 17;            /* fixed day of month in this sequence */
    int target_weekday = (0 + (TARGET_DAY - 1)) % 7;   /* weekday of TARGET_DAY
                                                        in the first month */

    idx_t month_counter = 0;              /* months elapsed since start */
    idx_t last_event_month = 0;           /* month of last event */
    size_t gaps_found = 0;
    int month_of_year = 0;                /* 0–11 */
    idx_t year = 0;                       /* year 0: leap (divisible by 4) */

    while (gaps_found < PERIOD) {
        int target_wday;
        int first_wday_unused;
        month_weekdays(year, month_of_year, TARGET_DAY,
                       &first_wday_unused, &target_wday);

        if (month_counter == 0) {         /* first month: always an event */
            last_event_month = 0;
        } else if (target_wday == target_weekday) {
            cycle[gaps_found++] = (seq_t)(month_counter - last_event_month);
            last_event_month = month_counter;
        }

        /* advance to next month */
        month_of_year++;
        month_counter++;
        if (month_of_year == 12) {
            month_of_year = 0;
            year++;
        }
    }
    cycle_built = true;
}

/* the public entry point – return the n‑th gap (n>=1) */
seq_t a(idx_t n) {
    if (n == 0) return 0;               /* undefined for n<1, harmless */
    build_cycle();
    idx_t idx = (n - 1) % (idx_t)PERIOD;
    return cycle[(size_t)idx];
}
