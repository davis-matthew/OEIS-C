/*
 * A088839  Numerator of sigma(4n)/sigma(n).
 * https://oeis.org/A088839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, frac
 * Formula:   a(n) = numerator( sigma(4n) / sigma(n) ) For n = 2^e * m with m
 *            odd, sigma(4n)/sigma(n) = (2^(e+3)-1) / (2^(e+1)-1) The function
 *            below computes this fraction and returns the reduced numerator
 * Range:     u8 n <= 63, u16 n <= 16383, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 108 terms through n = 9815 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required standard header */
#include <stdlib.h>
#include "../utils/pow2.h"

/* helper: gcd for seq_t */
static seq_t seq_gcd(seq_t a, seq_t b) {
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* main OEIS function */
seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n < 1 */

    /* exponent of 2 in n */
    idx_t e = 0;
    idx_t t = n;
    while ((t & 1) == 0) {
        ++e;
        t >>= 1;
    }

    /* compute numerator and denominator of the ratio */
    seq_t num = pow2(e + 3) - 1;   /* 2^(e+3)-1 */
    seq_t den = pow2(e + 1) - 1;   /* 2^(e+1)-1 */

    /* reduce the fraction */
    seq_t g = seq_gcd(num, den);
    return num / g;
}
