/*
 * A074598  Numerator of 4 * H(n,4,1), a generalized harmonic number. See
 *          A075136.
 * https://oeis.org/A074598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = numerator of 4 * \sum_{k=1}^{n} 1/(4k-3)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t num = 0;            /* current numerator of the sum */
    seq_t den = 1;            /* current denominator of the sum */
    const seq_t term_num = (seq_t)4; /* 4 in the formula */

    for (idx_t k = 1; k <= n; ++k) {
        /* denominator of the k-th term: 4k-3 */
        seq_t term_den = (seq_t)4 * k - (seq_t)3;

        /* Reduce the current denominator and the new term
           to keep the fraction in lowest terms before adding. */
        seq_t g1 = gcd(den, term_den);
        seq_t d1  = den / g1;
        seq_t d2  = term_den / g1;

        /* Add the fractions: num/den + term_num/term_den */
        seq_t new_num = num * d2 + term_num * d1;
        seq_t new_den = g1 * d1 * d2;      /* lcm of den and term_den */

        /* Reduce the result again to avoid overflow in future steps */
        seq_t g2 = gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    return num;          /* numerator of 4 * H(n,4,1) */
}
