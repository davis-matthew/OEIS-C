/*
 * A119783  Numerator of Sum_{i=1..n} i^3/(n-i+1).
 * https://oeis.org/A119783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator( sum_{i=1..n} i^3/(n-i+1) )
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t num = 0;   /* numerator of the sum */
    seq_t den = 1;   /* denominator of the sum */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t i_seq    = (seq_t)i;
        seq_t term_num = i_seq * i_seq * i_seq;               /* i^3 */
        seq_t term_den = (seq_t)(n - i + 1);                  /* denominator */

        /* add current term: num/den + term_num/term_den */
        seq_t new_num = num * term_den + term_num * den;
        seq_t new_den = den * term_den;

        /* reduce fraction */
        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }
    return num;   /* numerator of the reduced sum */
}
