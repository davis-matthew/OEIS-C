/*
 * A131687  Days of the year that are prime numbers in format mmdd (2-digit
 *          month followed by 2-digit day).
 * https://oeis.org/A131687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn, base, less, changed
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A131687: prime days of the year.
   a(n) is the n-th prime number that can be written in the form mmdd
   where mm is a two‑digit month (01‑12) and dd is a valid day for that
   month (Feb 29 is allowed).  The terms are ordered by month,
   then by day.  There are 59 such dates (including 02 29).  The
   function builds the list once and returns the n‑th element. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* Singleton cache of the prime days.  Build only once. */
    static bool built = false;
    static seq_t dates[64];   /* 64 is safely above 59 */
    static size_t count = 0;

    if (!built) {
        /* Days in each month; February has 29 to include leap‑year date. */
        const uint8_t days_in_month[13] = {
            0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
        };

        size_t pos = 0;
        for (uint8_t m = 1; m <= 12; ++m) {
            uint8_t maxd = days_in_month[m];
            for (uint8_t d = 1; d <= maxd; ++d) {
                unsigned int tmp = (unsigned int)m * 100u + (unsigned int)d;
                seq_t val = (seq_t)tmp;
                if (is_prime_small(val))
                    dates[pos++] = val;
            }
        }
        count = pos;
        built = true;
    }

    if (n == 0 || n > count)
        return 0;  /* undefined for out‑of‑range indices */

    return dates[n - 1];
}
