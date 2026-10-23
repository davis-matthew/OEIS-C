/*
 * A016833  a(n) = (4n+2)^9.
 * https://oeis.org/A016833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4n+2)^9
 * Range:     u8 none, u16 n <= 0, u32 n <= 2, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    seq_t base = ((seq_t)4 * (seq_t)n) + 2;
    return big_pow(base, (seq_t)9);
}
