/*
 * A141245  Numerators in expansion of (1+x-sqrt(1-x^2))/(x(1-x)).
 * https://oeis.org/A141245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = 1 + Σ_{j=1}^{⌊(n+1)/2⌋} ((2j-3)!! / odd_part(j!)) /
 *            2^{j+v2(j!)} The returned value is the numerator of the
 *            coefficient in the expansion of (1+x-√(1-x²))/(x(1-x)), after
 *            the fraction is reduced to a power‑of‑two denominator
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 22, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* count trailing zeros of a non‑zero 64‑bit integer */
static inline idx_t trailing_zeros(idx_t x)
{
    idx_t c = 0;
    while ((x & 1) == 0)
    {
        ++c;
        x >>= 1;
    }
    return c;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0)
        return 1;

    idx_t k = (n + 1) / 2;      /* number of terms in the sum */

    /* numerator accumulator and current exponent of the common power of two */
    seq_t ans_num = 1;          /* from the constant term 1/1 */
    idx_t ans_exp = 0;          /* exponent such that the fraction is ans_num / 2^ans_exp */

    /* running products that allow O(k) computation of each summand */
    seq_t dbl_fact = 1;         /* (2j-3)!!  */
    seq_t odd_fact = 1;         /* odd part of j!  */
    idx_t v2_fact = 0;          /* v2(j!)  */

    for (idx_t j = 1; j <= k; ++j)
    {
        /* update v2(j!) and odd_part(j) */
        idx_t tz = trailing_zeros(j);
        v2_fact += tz;
        odd_fact *= (j >> tz);           /* odd part of j */

        /* update (2j-3)!! for j > 1 */
        if (j > 1)
            dbl_fact *= (2 * j - 3);

        /* integer numerator of the summand */
        seq_t term_num = dbl_fact / odd_fact;

        /* exponent of the denominator for this summand */
        idx_t t = j + v2_fact;           /* t = j + v2(j!) */

        /* bring ans_num to the common exponent */
        if (t > ans_exp)
        {
            unsigned int shift = (unsigned int)(t - ans_exp);
            ans_num <<= shift;
            ans_exp = t;
        }

        /* add the term to the accumulated sum */
        unsigned int shift = (unsigned int)(ans_exp - t);
        ans_num += term_num << shift;
    }

    return ans_num;
}
