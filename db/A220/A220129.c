/*
 * A220129  1 followed by period 12: (1, 5, 3, 9, 1, 7, 1, 9, 3, 5, 1, 11)
 *          repeated; offset 0.
 * https://oeis.org/A220129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, and for n>0 a(n)=P[(n‑1) mod 12] with P = {1, 5, 3, 9,
 *            1, 7, 1, 9, 3, 5, 1, 11}
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pattern[12] = {1, 5, 3, 9, 1, 7, 1, 9, 3, 5, 1, 11};

    if (n == 0)
        return 1;
    return pattern[(size_t)((n - 1) % 12)];
}
