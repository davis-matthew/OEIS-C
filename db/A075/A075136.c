/*
 * A075136  Numerator of the generalized harmonic number H(n,4,1).
 * https://oeis.org/A075136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = Numerator of the partial sum 1/1 + 1/5 + 1/9 + 1/13 + ...
 *            + 1/(4n-3)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of A075136. */
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* The series starts with 1/1. */
    seq_t num = 1;   /* numerator of the current fraction */
    seq_t den = 1;   /* denominator of the current fraction */

    if (n == 1)     /* a(1) = 1 */
        return num;

    for (idx_t k = 2; k <= n; ++k) {
        /* term 1/(4k-3) */
        seq_t step = ((seq_t)k * 4) - 3;

        /* Add 1/step to the current fraction:
           num/den + 1/step = (num*step + den) / (den*step) */
        seq_t new_num = num * step + den;
        seq_t new_den = den * step;

        /* Reduce the fraction. */
        seq_t g = gcd(new_num, new_den);
        new_num /= g;
        new_den /= g;

        num = new_num;
        den = new_den;
    }

    return num;   /* numerator of the reduced fraction */
}
