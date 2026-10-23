/*
 * A017015  a(n) = (7*n + 2)^11.
 * https://oeis.org/A017015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 2)^11
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 7, u128 n <= 454,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    seq_t base = (seq_t)n * 7 + 2;
    return big_pow(base, 11);
}
