/*
 * A036150  a(n) = 2^n mod 163.
 * https://oeis.org/A036150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod 163
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n) {
    /* The multiplicative order of 2 modulo 163 is 162.
       Reducing the exponent modulo 162 keeps the value small
       and speeds up mod_pow for very large n. */
    idx_t exp = n % 162;
    return mod_pow((seq_t)2, exp, (seq_t)163);
}
