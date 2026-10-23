/*
 * A180296  Number of arrangements of n indistinguishable balls in n boxes
 *          with the maximum number of balls in any box equal to n-6.
 * https://oeis.org/A180296
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * binomial(n+4, n-2) for n > 12; a(7..12) are supplied
 *            explicitly
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute gcd for seq_t numbers. */
static seq_t _seq_gcd(seq_t a, seq_t b) {
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    /* Handle index zone below the official offset (n < 7). */
    if (n < 7) return 0;

    /* Explicit values for 7 ≤ n ≤ 12. */
    static const seq_t small[6] = {
        1ULL,   /* a(7) */
        1106ULL,/* a(8) */
        9912ULL,/* a(9) */
        27600ULL,/* a(10) */
        54450ULL,/* a(11) */
        96030ULL /* a(12) */
    };
    if (n <= 12) return small[n - 7];

    /* Compute C(n+4,6) = binomial(n+4, n-2). */
    seq_t binom = 1ULL;
    for (idx_t i = 1; i <= 6; ++i) {
        /* Numerator terms go from n+4 down to n-1. */
        seq_t num = n + 5 - i; /* (n+4) - (i-1) */
        seq_t den = i;

        /* Cancel common factors between current result and denominator. */
        seq_t g = _seq_gcd(binom, den);
        binom /= g;
        den   /= g;

        /* Cancel common factors between numerator and remaining denominator. */
        g = _seq_gcd(num, den);
        num /= g;
        den /= g;  /* den should now be 1 */

        /* Multiply the reduced numerator. */
        binom *= num;
    }

    /* Final formula: a(n) = n * binom(n+4,6). */
    return (seq_t)n * binom;
}
