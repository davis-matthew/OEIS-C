/*
 * A021313  Decimal expansion of 1/309.
 * https://oeis.org/A021313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = a(n-34) for n > 33. The sequence is the decimal expansion
 *            of 1/309, whose period is 34
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Periodic digits of 1/309 */
    static seq_t period[34];
    static bool defined = false;
    if (!defined)
    {
        idx_t rem = 1;                /* current remainder */
        for (idx_t i = 0; i < 34; ++i)
        {
            seq_t d = (seq_t)(rem * 10 / 309);   /* digit */
            period[i] = d;
            rem = (rem * 10) % 309;            /* new remainder */
        }
        defined = true;
    }
    /* n may be very large; use modulo 34 to get the position in period */
    idx_t pos = n % 34;
    return period[pos];
}
