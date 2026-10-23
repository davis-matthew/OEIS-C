/*
 * A140940  Number of 8 X 11 matrices with elements in 0..n with each row and
 *          each column in nondecreasing order. 8,11,n can be permuted, see
 *          formula.
 * https://oeis.org/A140940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{i=0}^{n-1} * ( binomial(n+18, 8+i) * i! ) /
 *            (n+18-i)^(n-i-1)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   binomial, seq_gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* constants for the fixed matrix dimensions */
    const idx_t p = 8;           /* number of rows (fixed) */
    const idx_t q = 11;          /* number of columns (fixed) */
    const idx_t r = n;           /* third parameter from OEIS */
    const idx_t s = p + q + r - 1;   /* s = n + 18 */

    seq_t num = 1;   /* numerator of the running product */
    seq_t den = 1;   /* denominator of the running product */
    seq_t fact = 1;  /* running factorial i! */

    for (idx_t i = 0; i < r; ++i)
    {
        if (i > 0)
            fact *= (seq_t)i;   /* i! */

        /* compute the i‑th term of the product */
        seq_t term_num = binomial(s, p + i);   /* binomial(s, 8+i) */
        term_num *= fact;                      /* times i! */

        idx_t exp = r - i - 1;
        seq_t term_den = (exp == 0) ? 1 : seq_pow((seq_t)(s - i), exp);

        /* incorporate the term into the running fraction */
        num *= term_num;
        den *= term_den;

        /* reduce the fraction to avoid growth */
        seq_t g = seq_gcd(num, den);
        if (g > 1)
        {
            num /= g;
            den /= g;
        }
    }

    /* the final denominator should be 1; the result is the numerator */
    return num;
}
