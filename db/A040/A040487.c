/*
 * A040487  Continued fraction for sqrt(510).
 * https://oeis.org/A040487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(510):
   a(0) = 22; for n ≥ 1, a(n) repeats the cycle
   {1, 1, 2, 1, 1, 44} of length 6 */
#include <stddef.h>
static const seq_t cf_period[6] = { (seq_t)1, (seq_t)1, (seq_t)2,
                                    (seq_t)1, (seq_t)1, (seq_t)44 };

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)22;
    else
        return cf_period[(size_t)((n - (idx_t)1) % (idx_t)6)];
}
