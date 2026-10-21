/*
 * A089639  Denominator of (5/2)*Sum_{i=1..n} (-1)^(i-1)/(i^3*C(2*i,i)).
 * https://oeis.org/A089639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of (5/2)*sum_{i=1..n}
 *            (-1)^(i-1)/(i^3*C(2i,i))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition (0/1 after multiplying by 5/2) */
    if (n == 0) return 1;

    seq_t numer_abs = 0;      /* absolute value of the running numerator */
    seq_t denom     = 1;      /* absolute value of the running denominator */
    bool numer_pos = true;    /* sign of the running numerator */

    seq_t binom = 1;          /* C(2i,i) for previous i, initialized for i = 0 */

    for (idx_t i = 1; i <= n; ++i) {
        /* compute C(2i,i) from C(2(i-1),(i-1)) */
        seq_t prod = binom * ((seq_t)(2*i)) * ((seq_t)(2*i-1));
        seq_t divn = (seq_t)i * (seq_t)i;
        binom = prod / divn;   /* exact division */

        /* denominator of the i-th term */
        seq_t d_i = ((seq_t)i * (seq_t)i * (seq_t)i) * binom;

        /* sign of the term: (+1) when i is odd, (-1) when even */
        bool term_pos = (i & 1);

        /* greatest common divisor of current denominator and d_i */
        seq_t g1 = gcd(denom, d_i);
        seq_t denom_div = denom / g1;      /* denom/g1  */
        seq_t d_div     = d_i   / g1;      /* d_i/g1    */

        /* part of numerator from previous sum */
        seq_t part = numer_abs * d_div;
        seq_t add  = denom_div;           /* absolute value of the new term */

        seq_t new_num_abs;
        bool   new_num_pos;

        if (numer_pos == term_pos) {
            new_num_abs = part + add;
            new_num_pos = numer_pos;
        } else {
            /* subtraction */
            if (part >= add) {
                new_num_abs = part - add;
                new_num_pos = numer_pos;
            } else {
                new_num_abs = add - part;
                new_num_pos = term_pos;
            }
        }

        /* new denominator before reducing */
        seq_t new_denom = denom_div * d_i;   /* denom/g1 * d_i = denom * d_i / g1 */

        /* reduce the fraction */
        seq_t g2 = gcd(new_num_abs, new_denom);
        numer_abs = new_num_abs / g2;
        denom     = new_denom   / g2;
        numer_pos = new_num_pos;

        if (numer_abs == 0) numer_pos = true;   /* zero has no sign */
    }

    /* multiply the accumulated sum by 5/2 */
    numer_abs = numer_abs * 5;
    denom     = denom        * 2;

    seq_t g3 = gcd(numer_abs, denom);
    numer_abs = numer_abs / g3;
    denom     = denom     / g3;

    /* a(0) is 1, handle zero numerator */
    if (numer_abs == 0) return 1;

    return denom;
}
