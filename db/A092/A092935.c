/*
 * A092935  a(1) = 1; a(n) = floor
 *          {(n+1)(n+2)(n+3)...(n+k)}/{(n-1)(n-2)(n-3)...(n-k)} for the least
 *          value of k.
 * https://oeis.org/A092935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute the first integer in OEIS sequence A092935.
   The definition is: a(n) is the smallest integer m such that
   floor( (n+1)(n+2)…(n+m) / (n-1)(n-2)…(n-m) ) is integral. */

/* Euclidean algorithm for seq_t values. */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* offset 1: a(1) = 1 */
    if (n == 1) return (seq_t)1;

    seq_t num = (seq_t)1;   /* numerator product */
    seq_t den = (seq_t)1;   /* denominator product */

    for (idx_t k = 1; ; ++k) {
        /* Multiply (n+k) into numerator and (n-k) into denominator. */
        seq_t x = (seq_t)n + (seq_t)k;   /* factor for numerator   */
        seq_t y = (seq_t)n - (seq_t)k;   /* factor for denominator */

        /* Reduce x with current denominator. */
        seq_t g1 = seq_gcd(x, den);
        if (g1 > 1) {
            x /= g1;
            den /= g1;
        }

        /* Reduce y with current numerator. */
        seq_t g2 = seq_gcd(y, num);
        if (g2 > 1) {
            y /= g2;
            num /= g2;
        }

        /* Update products. */
        num *= x;
        den *= y;

        /* Final reduction of common factors. */
        seq_t g3 = seq_gcd(num, den);
        if (g3 > 1) {
            num /= g3;
            den /= g3;
        }

        /* Integer quotient reached when denominator is 1. */
        if (den == 1) return num;
    }
}
