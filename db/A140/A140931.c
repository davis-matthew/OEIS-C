/*
 * A140931  Number of 8 X 10 matrices with elements in 0..n with each row and
 *          each column in nondecreasing order. 8,10,n can be permuted, see
 *          formula.
 * https://oeis.org/A140931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=0}^{7} C(n+17, n+i) * i! / (n+17-i)^{7-i}
 * Range:     u8 none, u16 none, u32 none, u64 n <= 5, u128 n <= 6, big any n
 * Helpers:   binomial, gcd, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A140931:
 * Number of 8×10 matrices with entries in 0..n, each row and column
 * in nondecreasing order. */

#include "../utils/binomial.h"
#include "../utils/gcd.h"
#include "../utils/pow_seq.h"

static const seq_t _fact[8] = {
    (seq_t)1,  /* 0! */
    (seq_t)1,  /* 1! */
    (seq_t)2,  /* 2! */
    (seq_t)6,  /* 3! */
    (seq_t)24, /* 4! */
    (seq_t)120,/* 5! */
    (seq_t)720,/* 6! */
    (seq_t)5040/* 7! */
};

seq_t a(idx_t n)
{
    const idx_t p = n;
    const idx_t r = 8;          /* number of rows */
    const idx_t s = n + 17;     /* s = p + q + r - 1, q = 10 */

    seq_t num = (seq_t)1;       /* cumulative numerator   */
    seq_t den = (seq_t)1;       /* cumulative denominator */

    for (idx_t i = (idx_t)0; i < r; ++i) {
        /* Binomial coefficient C(s, p+i) and multiply by i! */
        seq_t bin = binomial(s, p + i);
        seq_t term_num = bin * _fact[i];

        /* Power part (s-i)^{r-i-1} */
        seq_t base = (seq_t)(s - i);
        seq_t term_den = pow_seq(base, (int)(r - 1 - i));

        /* Reduce cross fractions to keep intermediate values small */
        seq_t g1 = gcd(term_num, den);
        if (g1 > (seq_t)1) {
            term_num /= g1;
            den     /= g1;
        }

        seq_t g2 = gcd(term_den, num);
        if (g2 > (seq_t)1) {
            term_den /= g2;
            num     /= g2;
        }

        /* Multiply the reduced parts */
        num *= term_num;
        den *= term_den;

        /* Final reduction of the fraction */
        seq_t g3 = gcd(num, den);
        if (g3 > (seq_t)1) {
            num /= g3;
            den /= g3;
        }
    }

    /* The remaining denominator must divide the numerator exactly */
    if (den != (seq_t)1) {
        num /= den;
    }

    return num;
}
