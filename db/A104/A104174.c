/*
 * A104174  Numerator of the fractional part of a harmonic number.
 * https://oeis.org/A104174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numerator of the fractional part of the n‑th harmonic
 *            number (H(n))
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 24, u64 n <= 46, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean algorithm for seq_t, works for all supported integer types. */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t num = 0;   /* numerator of the current reduced fraction */
    seq_t den = 1;   /* denominator of the current reduced fraction */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t kk = (seq_t)k;

        /* Compute lcm(den, kk) without overflow in each step. */
        seq_t g = seq_gcd(den, kk);
        seq_t lcm = den / g * kk;   /* lcm = den * kk / g */

        /* Update the numerator before reduction. */
        seq_t m     = kk / g;       /* factor for multiplying num */
        seq_t term  = den / g;      /* term to add: LCM/kk */
        seq_t new_num = num * m + term;

        /* Reduce fraction to lowest terms. */
        seq_t d = seq_gcd(new_num, lcm);
        num = new_num / d;
        den = lcm / d;
    }

    /* The fractional part numerator is num mod den. */
    return num % den;
}
