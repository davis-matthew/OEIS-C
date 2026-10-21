/*
 * A053401  Number of seconds in the time units: second, minute, hour, day,
 *          week, month, year, century, millennium, (a)eon.
 * https://oeis.org/A053401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, nonn, full
 * Formula:   piecewise definition of the number of seconds in common time
 *            units a(1) = 1 a(2) = 60 a(3) = 60·60 = 3600 a(4) = 24·3600 =
 *            86400 a(5) = 7·86400 = 604800 a(6) = 31556952 / 12 = 2629746
 *            a(7) = 31556952 a(8) = 100 · 31556952 = 3155695200 a(9) = 1000 ·
 *            31556952 = 31556952000 a(10) = 1 000 000 000 · 3...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 8, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uintptr_t, if needed */

seq_t a(idx_t n)
{
    /* Base units up to a week */
    if (n == 1) return (seq_t)1ULL;                      /* second          */
    if (n == 2) return (seq_t)60ULL;                     /* minute          */
    if (n == 3) return (seq_t)3600ULL;                    /* hour            */
    if (n == 4) return (seq_t)86400ULL;                   /* day             */
    if (n == 5) return (seq_t)604800ULL;                  /* week            */

    /* Common average year length in seconds (365.2425 days) */
    const seq_t year = (seq_t)31556952ULL;                /* average year    */

    /* Month is the average month length: year / 12 */
    if (n == 6) return year / (seq_t)12ULL;               /* average month   */

    /* Year itself */
    if (n == 7) return year;                              /* average year    */

    /* Larger units as multiples of the average year */
    if (n == 8) return year * (seq_t)100ULL;              /* century         */
    if (n == 9) return year * (seq_t)1000ULL;             /* millennium      */
    if (n ==10) return year * (seq_t)1000000000ULL;        /* aeon (1e9 years) */

    /* Undefined terms beyond the known units */
    return (seq_t)0;
}
