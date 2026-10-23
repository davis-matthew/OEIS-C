/*
 * A010218  Continued fraction for sqrt(174).
 * https://oeis.org/A010218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 13, a(n) = cycle[(n-1) mod 4] for n > 0, where cycle =
 *            [5, 4, 5, 26]
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t cycle[4] = {5, 4, 5, 26};

    if (n == 0u)
        return (seq_t)13;

    /* n > 0, so n-1u is safe */
    return cycle[(idx_t)((n - 1u) & 3u)];
}
