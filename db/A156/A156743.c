/*
 * A156743  Month (3=March, 4=April) in which Easter occurs in Gregorian
 *          calendar year, starting with 1901.
 * https://oeis.org/A156743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 if Easter falls in March, 4 if in April for Gregorian
 *            year 1900 + n
 * Range:     u8 n <= 12, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n<1, safeguard */

    seq_t year = (seq_t)1900 + (seq_t)n;   /* Gregorian year */

    /* Meeus/Jones/Butcher algorithm */
    seq_t a_ = year % 19;
    seq_t b  = year / 100;
    seq_t c  = year % 100;
    seq_t d  = b / 4;
    seq_t e  = b % 4;
    seq_t f  = (b + 8) / 25;
    seq_t g  = (b - f + 1) / 3;
    seq_t h  = (19 * a_ + b - d - g + 15) % 30;
    seq_t i  = c / 4;
    seq_t k  = c % 4;
    seq_t l  = (32 + 2 * e + 2 * i - h - k) % 7;
    seq_t m  = (a_ + 11 * h + 22 * l) / 451;
    seq_t month = (h + l - 7 * m + 114) / 31;

    return month;  /* 3 for March, 4 for April */
}
