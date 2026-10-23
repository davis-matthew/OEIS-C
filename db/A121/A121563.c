/*
 * A121563  Numerator of Sum_{i=1..n} i!/(i^i).
 * https://oeis.org/A121563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = numerator of \sum_{i=1}^{n} i! / i^i
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   seq_gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_gcd.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0) return 0;

    seq_t sum_num = 0;    /* Numerator of the running sum */
    seq_t sum_den = 1;    /* Denominator of the running sum */
    seq_t fact    = 1;    /* Current factorial i! */

    for (idx_t i = 1; i <= n; ++i) {
        /* Update factorial: fact *= i */
        fact *= (seq_t)i;

        /* term = i! / i^i  */
        seq_t term_num = fact;
        seq_t term_den = seq_pow((seq_t)i, i);

        /* Reduce the term */
        seq_t g = seq_gcd(term_num, term_den);
        term_num /= g;
        term_den /= g;

        /* Add term to the running sum *
         *   sum = sum_num/sum_den + term_num/term_den            */
        seq_t new_num = sum_num * term_den + term_num * sum_den;
        seq_t new_den = sum_den * term_den;

        /* Reduce the new fraction */
        g = seq_gcd(new_num, new_den);
        sum_num = new_num / g;
        sum_den = new_den / g;
    }

    return sum_num;   /* numerator of the simplified sum */
}
