/*
 * A013732  a(n) = 3^(3*n + 1).
 * https://oeis.org/A013732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(3*n + 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    idx_t exp = 3 * n + 1;
    return pow3(exp);
}
