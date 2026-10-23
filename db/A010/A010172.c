/*
 * A010172  Continued fraction for sqrt(106).
 * https://oeis.org/A010172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   Continued fraction for sqrt(106)
 * Range:     u8 n <= 999, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = floor(sqrt(106)) = 10, the remaining terms repeat
   the period [3, 2, 1, 1, 1, 1, 2, 3, 20] */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 10;

    /* The periodic part of the continued fraction for sqrt(106) */
    static const seq_t period[9] = {
        (seq_t)3, (seq_t)2, (seq_t)1, (seq_t)1,
        (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)3,
        (seq_t)20
    };

    /* n-1 is safe because n != 0 here */
    idx_t idx = (n - 1) % 9;
    return period[(size_t)idx];
}
