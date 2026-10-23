/*
 * A017303  a(n) = (10*n + 2)^11.
 * https://oeis.org/A017303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n+2)^11
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 5, u128 n <= 318,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Compute the base (10*n + 2) in the appropriate integer type */
    seq_t base = ((seq_t)n * (seq_t)10) + (seq_t)2;
    /* Raise base to the 11th power using the provided big integer routine */
    return big_pow(base, (seq_t)11);
}
