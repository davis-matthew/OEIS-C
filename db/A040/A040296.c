/*
 * A040296  Continued fraction for sqrt(314).
 * https://oeis.org/A040296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(314).
   a(0) = 17
   a(n) = [1,2,1,1,2,1,34][(n-1) mod 7] for n ≥ 1   (period 7) */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Periodic partial quotients after the initial term */
    static const seq_t period[7] = { (seq_t)1, (seq_t)2, (seq_t)1,
                                     (seq_t)1, (seq_t)2, (seq_t)1,
                                     (seq_t)34 };

    if (n == 0)
        return (seq_t)17;

    /* Compute position in the period */
    idx_t pos = (n - 1) % (idx_t)7;
    return period[pos];
}
