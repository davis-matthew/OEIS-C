/*
 * A056001  a(n) = (n+1)*binomial(n+7, 7).
 * https://oeis.org/A056001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1) * binomial(n+7, 7)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 42, u64 n <= 739,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean algorithm for gcd on arbitrary-sized seq_t */
static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Compute C(n+7, 7) multiplicatively with fraction reduction to avoid
       intermediate blow‑up.  All arithmetic is on seq_t, which may be
       unsigned char, unsigned short, uint32_t, uint64_t, unsigned __int128,
       or an arbitrary‑precision integer type. */
    seq_t binom = 1;

    for (idx_t i = 1; i <= 7; ++i) {
        /* Numerator (n + i) and denominator i */
        seq_t num = (seq_t)(n + i);
        seq_t den = (seq_t)i;

        /* Reduce numerator with denominator */
        seq_t g = seq_gcd(num, den);
        if (g > 1) {
            num /= g;
            den /= g;
        }

        /* Reduce current binomial with remaining denominator */
        g = seq_gcd(binom, den);
        if (g > 1) {
            binom /= g;
            den /= g;
        }

        /* At this point den should be 1; multiply the remaining part */
        binom = binom * num;
    }

    /* Multiply by (n + 1) to get the final sequence value */
    seq_t result = binom * ((seq_t)n + 1);
    return result;
}
