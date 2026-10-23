/*
 * A186420  a(n) = binomial(2n,n)^4.
 * https://oeis.org/A186420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n,n)^4 (OEIS A186420)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* compute C(2n,n) using the multiplicative formula with gcd reduction */
    seq_t c = 1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t a_val = (seq_t)n + i; /* n + i  */
        seq_t b_val = i;            /* i      */

        /* cancel common factors to keep numbers small */
        seq_t g = gcd(a_val, b_val);
        a_val /= g;
        b_val /= g;

        /* divide current result by b_val and multiply by a_val */
        c = (c / (seq_t)b_val) * a_val;
    }

    /* raise to the fourth power */
    c = c * c;  /* square once */
    c = c * c;  /* square twice => fourth power */
    return c;
}
