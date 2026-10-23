/*
 * A021428  Decimal expansion of 1/424.
 * https://oeis.org/A021428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = (10 * (10^n mod 424)) / 424
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n) {
    /* Compute 10^n modulo 424 */
    seq_t rem = mod_pow((seq_t)10, n, (seq_t)424);

    /* The next digit is the integer part of (10 * rem) / 424 */
    seq_t digit = (rem * (seq_t)10) / (seq_t)424;

    return (seq_t)digit;
}
