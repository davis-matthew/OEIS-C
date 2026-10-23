/*
 * A174515  Expansion of g.f. exp( Sum_{n>=1} (2^n+3^n)^2*x^n/n ).
 * https://oeis.org/A174515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9^(2+n) - 4^(2+n) - 5*6^(1+n)*(2 + n))/5
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast exponentiation by squaring: computes base^exp where exp fits in idx_t */
static seq_t pow_uint_fast(uint64_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp) {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t pow9 = pow_uint_fast(9, n + 2);
    seq_t pow4 = pow_uint_fast(4, n + 2);
    seq_t pow6 = pow_uint_fast(6, n + 1);

    seq_t tmp = pow6 * ((seq_t)n + 2);
    tmp *= 5;
    seq_t term = pow9 - pow4 - tmp;

    return term / 5;
}
