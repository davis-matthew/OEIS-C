/*
 * A017142  a(n) = (8*n + 6)^6.
 * https://oeis.org/A017142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 6)^6
 * Range:     u8 none, u16 n <= 0, u32 n <= 4, u64 n <= 202, u128 n <= 10000,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t bn = (seq_t)n;
    seq_t base = (seq_t)8 * bn + 6;
    return big_pow(base, (seq_t)6);
}
