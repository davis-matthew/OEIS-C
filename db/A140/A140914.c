/*
 * A140914  Number of 5 X 8 matrices with elements in 0..n with each row and
 *          each column in nondecreasing order. 5,8,n can be permuted, see
 *          formula.
 * https://oeis.org/A140914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   binom, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (empirical for this sequence):
   a(n) = product_{i=0}^{r-1} ( binomial(s, p+i) * i! / (s-i)^(r-i-1) )
   with p = n, q = 8, r = 5, s = p + q + r - 1.
   The result is always an integer.
*/

#include <stdint.h>
#include <stddef.h>
#include "../utils/binom.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Parameters of the formula */
    const idx_t p = n;          /* variable part of the parameters */
    const idx_t q = 8;          /* fixed part */
    const idx_t r = 5;          /* fixed part */
    const idx_t s = p + q + r - 1;   /* (n + 12) */

    /* Pre-compute factorials 0! .. (r-1)! */
    seq_t fact[5];
    fact[0] = 1;
    for (idx_t i = 1; i < r; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* Running numerator and denominator of the product */
    seq_t num = 1;
    seq_t den = 1;

    for (idx_t i = 0; i < r; ++i) {
        /* Compute the i‑th factor */

        /* binomial(s, p + i) */
        seq_t b = binom(s, p + i);

        /* i! factor */
        seq_t i_fact = fact[i];

        /* denominator (s-i)^(r-i-1) as a product loop */
        seq_t den_i = 1;
        for (idx_t e = 0; e < r - i - 1; ++e)
            den_i *= (seq_t)(s - i);

        /* Numerator of the factor */
        seq_t fact_num = b * i_fact;

        /* Reduce before multiplying to keep intermediates small */

        /* Reduce fact_num with current den */
        seq_t g1 = gcd(fact_num, den);
        if (g1 > 1) {
            fact_num /= g1;
            den /= g1;
        }

        /* Reduce num with den_i */
        seq_t g2 = gcd(num, den_i);
        if (g2 > 1) {
            num /= g2;
            den_i /= g2;
        }

        /* Multiply the reduced parts */
        num *= fact_num;
        den *= den_i;

        /* Final reduction (denominator should divide numerator) */
        seq_t g3 = gcd(num, den);
        if (g3 > 1) {
            num /= g3;
            den /= g3;
        }
    }

    /* The result is an integer, so den should be 1 */
    return num / den;
}
