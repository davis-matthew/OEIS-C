/*
 * A016786  a(n) = (3*n+1)^10.
 * https://oeis.org/A016786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n+1)^10
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 27, u128 n <= 2376,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 3 + 1;
    return big_pow(base, (seq_t)10);
}
