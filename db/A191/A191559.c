/*
 * A191559  a(n) = 25^(4*n+1).
 * https://oeis.org/A191559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25^(4*n+1)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    idx_t exp = 4 * n + 1;
    seq_t base = 25;
    return pow_int(base, exp);
}
