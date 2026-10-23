/*
 * A017278  a(n) = (10*n)^10.
 * https://oeis.org/A017278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n)^10
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 8, u128 n <= 500,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    seq_t base = ((seq_t)n) * ((seq_t)10);
    return pow_int(base, (idx_t)10);
}
