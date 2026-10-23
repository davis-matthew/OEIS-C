/*
 * A040330  Continued fraction for sqrt(349).
 * https://oeis.org/A040330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 18; for n ≥ 1, a(n) = P[(n-1) mod 7], where P = {1, 2, 7,
 *            7, 2, 1, 36}
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)18;

    /* period of the continued fraction after the initial term */
    static const seq_t period[7] = {1, 2, 7, 7, 2, 1, 36};

    /* compute (n-1) mod 7 and index into the period array */
    size_t idx = (size_t)((n - 1) % 7);
    return period[idx];
}
