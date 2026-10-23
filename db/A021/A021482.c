/*
 * A021482  Decimal expansion of 1/478.
 * https://oeis.org/A021482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(0) = 0; for n ≥ 1, a(n) = pattern[(n‑1) mod 7] where pattern =
 *            {0, 2, 0, 9, 2, 0, 5}
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }
    static const seq_t pattern[7] = {0, 2, 0, 9, 2, 0, 5};
    return pattern[(size_t)((n - 1) % 7)];
}
