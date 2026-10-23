/*
 * A163599  The smallest pronic (A002378) with the same most significant
 *          digits as n.
 * https://oeis.org/A163599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest pronic number (m*(m+1)) whose most significant
 *            digits equal n
 * Range:     u8 n <= 2, u16 n <= 31, u32 n <= 3978, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Keeps searching over numbers with the same number of digits as n
   until a suitable pronic is found. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;         /* Index as seq_t for arithmetic */
    seq_t pow10 = 1;            /* 10^k, k starts at 0 */

    for (;;) {
        /* Range of numbers with most significant digits n:
           [low, high) where low = n * 10^k and high = (n+1)*10^k. */
        seq_t low  = N * pow10;
        seq_t high = (N + (seq_t)1) * pow10;

        /* Find smallest m such that m*(m+1) >= low.
           Compute discriminant of m^2 + m - low = 0. */
        seq_t four_low = (seq_t)4 * low;
        seq_t disc     = four_low + (seq_t)1;
        seq_t sq       = floor_sqrt(disc); /* floor(sqrt(disc)) */
        seq_t m        = (sq > (seq_t)0) ? ((sq - (seq_t)1) / (seq_t)2)
                                         : (seq_t)0;

        seq_t pronic = m * (m + (seq_t)1);
        while (pronic < low) {        /* ensure we have the ceiling */
            ++m;
            pronic = m * (m + (seq_t)1);
        }

        /* If it lies inside the interval we are done. */
        if (pronic < high)
            return pronic;

        /* Otherwise enlarge the interval by one more decimal digit. */
        pow10 = pow10 * (seq_t)10;
    }
}
