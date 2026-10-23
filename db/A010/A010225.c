/*
 * A010225  Continued fraction for sqrt(183).
 * https://oeis.org/A010225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Pattern for the continued fraction of sqrt(183):
   a(0) = 13
   for n >= 1, a(n) cycles through {1, 1, 8, 1, 1, 26}. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)13;

    static const seq_t pattern[6] = { (seq_t)1, (seq_t)1, (seq_t)8,
                                     (seq_t)1, (seq_t)1, (seq_t)26 };
    return pattern[(size_t)((n - 1) % 6)];
}
