/*
 * A130426  a(n) = denominator of Sum_{k=1..n} 1/k^(n+1-k).
 * https://oeis.org/A130426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = denominator of Sum_{k=1..n} 1/k^(n+1-k)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* numerator and denominator of the running sum */
    seq_t num = 0;
    seq_t den = 1;

    for (idx_t k = 1; k <= n; ++k) {
        /* exponent for this term: n+1-k   */
        seq_t exp = (seq_t)(n + 1 - k);

        /* denominator of the single term 1/k^exp */
        seq_t term_den = big_pow((seq_t)k, exp);

        /* common divisor to keep numbers small */
        seq_t g = gcd(den, term_den);
        seq_t den_div_g = den / g;

        /* update the fraction: num/den + 1/term_den   */
        seq_t t = term_den / g;          /* term_den / gcd */
        seq_t new_num = num * t + den_div_g;
        seq_t new_den = den_div_g * term_den;

        /* reduce the fraction to lowest terms */
        seq_t g2 = gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    return den;
}
