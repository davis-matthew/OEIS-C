/*
 * A040289  Continued fraction for sqrt(307).
 * https://oeis.org/A040289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 17; for n > 0, a(n) = period[(n-1) mod 14] where period =
 *            {1, 1, 11, 5, 1, 3, 17, 3, 1, 5, 11, 1, 1, 34}
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)17U;

    /* Periodic part of the continued‑fraction expansion of sqrt(307).
       The period length is 14. */
    static const seq_t period[14] = {
        1, 1, 11, 5, 1, 3, 17, 3,
        1, 5, 11, 1, 1, 34
    };

    idx_t r = (n - 1) % 14;
    return period[(size_t)r];
}
