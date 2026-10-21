/*
 * A220128  1 followed by period 6: (1, 3, 2, 3, 1, 4) repeated; offset 0.
 * https://oeis.org/A220128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n > 0, a(n) = pattern (1, 3, 2, 3, 1, 4) repeating
 *            (i.e., a(n) = pattern[(n-1) mod 6])
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    static const seq_t pattern[6] = {1, 3, 2, 3, 1, 4};
    return pattern[(size_t)((n - 1) % 6)];
}
