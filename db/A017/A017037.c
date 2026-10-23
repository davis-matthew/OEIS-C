/*
 * A017037  a(n) = (7*n + 4)^9.
 * https://oeis.org/A017037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 4)^9
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 19, u128 n <= 2729,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    seq_t base = (seq_t)7 * (seq_t)n + (seq_t)4;
    return big_pow(base, (seq_t)9);
}
