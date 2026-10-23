/*
 * A118431  Numerator of sum of reciprocals of first n 5-simplex numbers
 *          A000389.
 * https://oeis.org/A118431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = numerator of Σ_{i=1..n} 1/C(i,5) = Σ_{i=1..n}
 *            120/(i(i+1)(i+2)(i+3)(i+4))
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 378, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset is 1, but keep safe for n=0 */

    seq_t num = 0;      /* current numerator of the sum */
    seq_t den = 1;      /* current denominator of the sum (in lowest terms) */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t i_s = (seq_t)i;
        /* denominator of the new term: i*(i+1)*(i+2)*(i+3)*(i+4) */
        seq_t term_den = i_s * (i_s + 1) * (i_s + 2) * (i_s + 3) * (i_s + 4);
        seq_t term_num = 120; /* numerator of the new term */

        /* compute lcm of den and term_den without overflow when possible */
        seq_t g = gcd(den, term_den);
        seq_t den1 = den / g;
        seq_t den2 = term_den / g;          /* term_den/g */

        /* new numerator and denominator before reducing */
        seq_t new_num = num * den2 + term_num * den1;
        seq_t new_den = den * den2;         /* lcm(den, term_den) */

        /* reduce the fraction to lowest terms */
        seq_t r = gcd(new_num, new_den);
        num = new_num / r;
        den = new_den / r;
    }

    return num;
}
