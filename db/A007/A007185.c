/*
 * A007185  Automorphic numbers ending in digit 5: a(n) = 5^(2^n) mod 10^n.
 * https://oeis.org/A007185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 5^n * k, where k = 5^(-n) mod 2^n (the unique 0 < a(n) <
 *            10^n with 5^n | a(n) and a(n) == 1 (mod 2^n))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   a007185_pow5, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/a007185_pow5.h"

seq_t a(idx_t n) {
    seq_t mask;
    if (seq_is_fixed_width() && n >= (idx_t)(8 * sizeof(seq_t)))
        mask = (seq_t)0 - 1;
    else
        mask = ((seq_t)1 << n) - 1;

    seq_t p = a007185_pow5(n);
    seq_t c = p & mask;

    /* Newton iteration for inverse of odd c modulo 2^n: x <- x*(2 - c*x) */
    seq_t x = c;          /* correct modulo 2^3 for odd c */
    idx_t bits = 3;
    while (bits < n) {
        seq_t t = (c * x) & mask;
        seq_t f = ((mask - t) + 3) & mask;   /* (2 - t) mod 2^n */
        x = (x * f) & mask;
        bits *= 2;
    }
    x &= mask;

    return p * x;
}
