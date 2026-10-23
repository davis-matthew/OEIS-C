/*
 * A199026  a(n) = 7*11^n-1.
 * https://oeis.org/A199026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*11^n - 1 Recurrence: a(0) = 6; a(n) = 11*a(n-1) + 10
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t value = 6;   /* a(0) */
    if (n == 0)
        return value;

    /* Iterate using the recurrence; intermediate overflow is fine
       because larger values do not fit in seq_t anyway. */
    for (idx_t i = 1; i <= n; ++i) {
        value = value * 11 + 10;
    }
    return value;
}
