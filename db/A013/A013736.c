/*
 * A013736  a(n) = 5^(3*n + 1).
 * https://oeis.org/A013736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(3*n + 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 8, u128 n <= 18,
 *            big any n
 * Helpers:   power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/power5.h"

seq_t a(idx_t n)
{
    idx_t exp = 3*n + 1;
    return power5(exp);
}
