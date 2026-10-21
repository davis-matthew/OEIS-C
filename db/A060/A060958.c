/*
 * A060958  Gregorian Easter dates, starting after 2000-04-23, a(2000) = 54.
 * https://oeis.org/A060958
 *
 * Offset:    2001    (a(2001) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (month == 3 ? day_of_month : 31 + day_of_month) where
 *            month and day_of_month are the Gregorian Easter month and day
 *            for year n
 * Range:     u8 n <= 2066, u16 n <= 2066, u32 n <= 2066, u64 n <= 2066,
 *            u128 n <= 2066, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Gregorian Easter computation using the standard algorithm. */
#include <stdint.h>
seq_t a(idx_t n)
{
    uint64_t Y = (uint64_t)n;
    uint64_t a1 = Y % 19;
    uint64_t b  = Y / 100;
    uint64_t c  = Y % 100;
    uint64_t d  = b / 4;
    uint64_t e  = b % 4;
    uint64_t f  = (b + 8) / 25;
    uint64_t g  = (b - f + 1) / 3;
    uint64_t h  = (19 * a1 + b - d - g + 15) % 30;
    uint64_t i  = c / 4;
    uint64_t k  = c % 4;
    uint64_t l  = (32 + 2 * e + 2 * i - h - k) % 7;
    uint64_t m  = (a1 + 11 * h + 22 * l) / 451;
    uint64_t month = (h + l - 7 * m + 114) / 31;
    uint64_t day   = (h + l - 7 * m + 114) % 31 + 1;

    uint64_t result = (month == 3) ? day : (31 + day);
    return (seq_t)result;
}
