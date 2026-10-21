/*
 * A095664  Ninth column (m=8) of (1,3)-Pascal triangle A095660.
 * https://oeis.org/A095664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+7,7)*(n+24)/8
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
seq_t a(idx_t n) {
    seq_t comb = binom(n + 7, 7);
    seq_t prod = comb * ((seq_t)n + 24);
    return prod / 8;
}
