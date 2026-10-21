/*
 * A210893  Dates after Jan 01 1000 in chronological order which are
 *          palindromic when they are written according to the format MMDDYYYY
 *          (American standard). Leading zeros of the terms are suppressed.
 * https://oeis.org/A210893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = palindromic MMDDYYYY date after 01‑01‑1000, indexed
 *            starting at 1. Computation is by brute‑force enumeration of
 *            dates and checking the palindrome condition
 * Range:     u8 none, u16 none, u32 n <= 331, u64 n <= 331, u128 n <= 331,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* check leap year for years between 1000 and 9999 */
static bool is_leap_year(idx_t year) {
    return (year % 400ULL == 0) || ((year % 4ULL == 0) && (year % 100ULL != 0));
}

/* number of days in month (month 1..12) for given year */
static size_t days_in_month(idx_t year, idx_t month) {
    static const size_t dim[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (month == 2 && is_leap_year(year))
        return 29;
    return dim[month - 1];
}

seq_t a(idx_t n) {
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    if (seq == NULL) {
        /* allocate initial capacity */
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        for (idx_t year = 1000; year <= 9999; ++year) {
            for (idx_t month = 1; month <= 12; ++month) {
                size_t dim = days_in_month(year, month);
                for (idx_t day = 1; day <= dim; ++day) {
                    /* compute year from month/day to check palindrome property */
                    idx_t rev_year =
                        (day % 10ULL) * 1000ULL +
                        (day / 10ULL) * 100ULL +
                        (month % 10ULL) * 10ULL +
                        (month / 10ULL);
                    if (rev_year == year) {
                        seq_t val = month * 1000000ULL + day * 10000ULL + year;
                        if (seq_len == seq_cap) {
                            seq_cap <<= 1;
                            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                        }
                        seq[seq_len++] = val;
                    }
                }
            }
        }
    }
    if (n == 0 || n > seq_len) return 0;  /* out of bounds */
    return seq[n - 1];
}
