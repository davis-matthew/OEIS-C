/*
 * A017518  a(n) = (11*n + 10)^10.
 * https://oeis.org/A017518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 10)^10
 * Range:     u8 none, u16 none, u32 none, u64 n <= 6, u128 n <= 647,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * (seq_t)11 + (seq_t)10;
    return big_pow(base, (seq_t)10);
}
