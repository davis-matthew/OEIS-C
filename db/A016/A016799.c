/*
 * A016799  a(n) = (3*n + 2)^11.
 * https://oeis.org/A016799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n + 2)^11
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 18, u128 n <= 1000,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    /* Convert n to the proper integer type first */
    seq_t x = (seq_t)n;
    seq_t base = x * (seq_t)3 + (seq_t)2;
    return seq_pow(base, (idx_t)11);
}
