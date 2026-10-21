/*
 * A073595  Denominator of b(n) = Sum_{k=1..n} (-1)^(k+1)/k*Sum_{i=0..k-1}
 *          (-1)^i/(2*i+1).
 * https://oeis.org/A073595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = denominator of * b(n) = Σ_{k=1..n} (-1)^(k+1)/k *
 *            Σ_{i=0..k-1} (-1)^i/(2*i+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 32,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static void add_frac(seq_t *num, seq_t *den, seq_t n, seq_t d, int sign)
{
    /* Add (sign)*n/d to *num/*den. Negative sign indicates subtraction. */
    seq_t g = gcd(*den, d);
    seq_t den1 = *den / g;       /* *den/g  */
    seq_t den2 = d / g;          /* d/g     */
    seq_t new_den = den1 * d;    /* lcm(*den,d) */
    seq_t left = *num * den2;    /* *num * (d/g)  */
    seq_t right = n * den1;      /* n * (*den/g)  */
    seq_t new_num;
    if (sign >= 0) {
        new_num = left + right;
    } else {
        /* Assume left >= right (sum stays positive) */
        new_num = (left >= right) ? left - right : right - left;
    }
    seq_t g2 = gcd(new_num, new_den);
    *num = new_num / g2;
    *den = new_den / g2;
}

seq_t a(idx_t n)
{
    /* Inner sum S(k) = Σ_{i=0..k-1} (-1)^i/(2*i+1) */
    seq_t S_num = 1;   /* numerator of S */
    seq_t S_den = 1;   /* denominator of S */

    /* Outer sum b(n) */
    seq_t b_num = 0;   /* numerator of b */
    seq_t b_den = 1;   /* denominator of b */

    for (idx_t k = 1; k <= n; ++k) {
        if (k > 1) {
            /* Add the new term to the inner sum */
            idx_t i = k - 1;
            seq_t term_num = 1;
            seq_t term_den = (seq_t)(2 * i + 1);
            int term_sign = ((i & 1) == 0) ? 1 : -1;  /* (+1) for even i, (-1) for odd i */
            add_frac(&S_num, &S_den, term_num, term_den, term_sign);
        }

        /* Outer term: (-1)^(k+1)/k * S(k) */
        seq_t t_num = S_num;
        seq_t t_den = S_den * (seq_t)k;  /* denominator of the term */
        int outer_sign = ((k & 1) == 1) ? 1 : -1;  /* + for odd k, - for even k */
        add_frac(&b_num, &b_den, t_num, t_den, outer_sign);
    }
    return b_den;
}
