/*
 * A014390  Final 2 digits of 7^n.
 * https://oeis.org/A014390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n mod 50
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* 7^n mod 50, using the provided modular exponentiation routine */
    return mod_pow((seq_t)7, n, (seq_t)50);
}
