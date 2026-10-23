/*
 * A040263  Continued fraction for sqrt(280).
 * https://oeis.org/A040263
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

/* Partial quotients of the continued fraction for √280.
   a(0) = 16.
   For n > 0: a(n) = {1, 2, 1, 2, 1, 32}[(n - 1) % 6]   */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)16;

    static const seq_t cycle[6] = {
        (seq_t)1, (seq_t)2, (seq_t)1,
        (seq_t)2, (seq_t)1, (seq_t)32
    };

    return cycle[(size_t)((n - 1) % 6)];
}
