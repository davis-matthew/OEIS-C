/*
 * A040663  Continued fraction for sqrt(690).
 * https://oeis.org/A040663
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

/* Continued fraction terms of sqrt(690):
   a(0) = 26;
   for n >= 1, a(n) = period[(n-1) mod 6] where period = {3,1,2,1,3,52} */
#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t period[6] = {3, 1, 2, 1, 3, 52};

    if (n == 0)
        return 26;

    /* (n-1) is guaranteed to be non‑negative here, modulo 6 fits in idx_t */
    size_t idx = (size_t)((n - 1) % 6U);
    return period[idx];
}
