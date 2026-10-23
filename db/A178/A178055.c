/*
 * A178055  Numbers representing the number of days in a month in the
 *          Gregorian calendar (modulus 7).
 * https://oeis.org/A178055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (days_in_month(n) - 28) mod 7, where days_in_month
 *            follows the * Gregorian calendar starting with January 2000. The
 *            sequence repeats every * 400 years (4800 months)
 * Range:     u8 n <= 4800, u16 n <= 4800, u32 n <= 4800, u64 n <= 4800,
 *            u128 n <= 4800, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* month index 0‑11, year offset from 2000 */
    idx_t month = (n - 1) % 12;
    idx_t years_since_2000 = (n - 1) / 12;
    idx_t year_mod400 = years_since_2000 % 400;

    /* Determine if the year (2000 + years_since_2000) is a leap year.
       The leap‑year pattern repeats every 400 years, so we may use
       the remainder modulo 400. */
    bool leap = (year_mod400 % 400 == 0) ||
                ((year_mod400 % 4 == 0) && (year_mod400 % 100 != 0));

    unsigned days;
    switch ((int)month) {
    case 1: /* February */
        days = 28 + (leap ? 1 : 0);
        break;
    case 3: case 5: case 8: case 10: /* April, June, September, November */
        days = 30;
        break;
    case 0: case 2: case 4: case 6: case 7: case 9: case 11: /* All others */
        days = 31;
        break;
    default:               /* should never happen */
        days = 31;
        break;
    }

    /* days - 28 is already 0,1,2,3.  Cast to seq_t before returning. */
    return (seq_t)(days - 28);
}
