/*
 * A134805  Denominator of Sum_{i=1..n} 1/(i^2*binomial(2*i,i)).
 * https://oeis.org/A134805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of Σ_{i=1..n} 1/(i^2*binomial(2*i,i))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1 */

#include <stdint.h>
#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) */
#include "../utils/gcd.h"        /* gcd(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t num = 0;          /* numerator of current sum */
    seq_t den = 1;          /* denominator of current sum */

    for (idx_t i = 1; i <= n; ++i)
    {
        idx_t two_i = i + i;                    /* 2*i for binomial */
        seq_t bin = binomial(two_i, i);         /* binomial(2*i,i) */

        seq_t term_den = bin;
        term_den = term_den * (seq_t)i * (seq_t)i;  /* i^2 * bin */

/* Reduce term with current denominator */
        seq_t g1 = gcd(den, term_den);
        seq_t den_div   = den   / g1;
        seq_t term_div  = term_den / g1;

/* Add fractions: (num/den) + (1/term_den) */
        seq_t new_num = num * term_div + den_div;
        seq_t new_den = den * term_div;

        seq_t g2 = gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    return den;
}
