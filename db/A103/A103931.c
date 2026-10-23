/*
 * A103931  Denominators of squares of harmonic numbers A001008/A002805.
 * https://oeis.org/A103931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = denominator(H(n))^2, where H(n) is the nth harmonic number.
 * The denominator of H(n) is obtained by adding the fractions 1/k sequentially
 * and reducing each intermediate sum to lowest terms.  The denominator of
 * H(n)^2 is the square of this final denominator.
 */

#include <stdbool.h>

/* Euclidean gcd for any seq_t type that supports % and == */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        /* undefined for n=0, return 0 */
        return (seq_t)0;
    }

    seq_t num = (seq_t)0;   /* numerator of the running sum */
    seq_t den = (seq_t)1;   /* denominator of the running sum */

    for (idx_t k = (idx_t)1; k <= n; ++k) {
        seq_t K   = (seq_t)k;
        seq_t new_num = num * K + den;
        seq_t new_den = den * K;

        seq_t g = gcd_seq(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    /* denominator of H(n) is den; its square is the required term */
    return den * den;
}
