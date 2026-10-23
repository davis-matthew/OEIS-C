/*
 * A017275  a(n) = (10*n)^7.
 * https://oeis.org/A017275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n)^7
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 56, u128 n <= 10000,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    /* Calculate (10 * n)^7 using the provided seq_pow helper */
    seq_t base = (seq_t)10 * n;
    return seq_pow(base, 7U);
}
