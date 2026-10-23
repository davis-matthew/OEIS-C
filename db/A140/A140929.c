/*
 * A140929  Number of 6 X 10 matrices with elements in 0..n with each row and
 *          each column in nondecreasing order. 6,10,n can be permuted, see
 *          formula.
 * https://oeis.org/A140929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = ∏_{i=0}^{5} binomial(n+15, n+i) * i! / (n+15-i)^(5-i)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 7, u128 n <= 8, big any n
 * Helpers:   binomial, factorial, seq_gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/factorial.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* Parameters for the 6×10 matrices – p = n, q = 10, r = 6 */
    const idx_t p = n;
    const idx_t q = 10;
    const idx_t r = 6;
    const idx_t s = p + q + r - 1;           /* s = n+15 */

    seq_t res_num = 1;  /* current numerator */
    seq_t res_den = 1;  /* current denominator */

    for (idx_t i = 0; i < r; ++i) {
        /* term = binomial(s, p+i) * i! / (s-i)^(r-i-1) */
        seq_t bin = binomial(s, p + i);
        seq_t fact = factorial(i);
        seq_t num = bin;
        num *= fact;                                   /* numerator */

        seq_t den = seq_pow((seq_t)(s - i), r - i - 1); /* denominator */

        /* Reduce the fraction of this term */
        seq_t g = seq_gcd(num, den);
        num /= g;
        den /= g;

        /* Cross‑reduce with the accumulated numerator/denominator */
        g = seq_gcd(num, res_den);
        num /= g;
        res_den /= g;

        g = seq_gcd(res_num, den);
        res_num /= g;
        den /= g;

        /* Multiply into the accumulator */
        res_num *= num;
        res_den *= den;
    }

    /* The final denominator should be 1 – just in case */
    if (res_den != (seq_t)1) {
        res_num /= res_den;
    }

    return res_num;
}
