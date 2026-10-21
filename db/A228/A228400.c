/*
 * A228400  The number of permutations of length n sortable by 3 cut-and-paste
 *          moves.
 * https://oeis.org/A228400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=0}^{20} p_i * binom(n - i + 9, 9) where p_i are the
 *            coefficients from the generating function P(x) = -x*(2*x^19 +
 *            28*x^17 - 90*x^16 + 31*x^15 - 329*x^14 + 2874*x^13 - 1487*x^12 -
 *            13363*x^11 + 17425*x^10 + 8876*x^9 - 16945*x^8 - 8185*x^7 -
 *            1326*x^6 - 48*x^5 - 120*x^4 + 66*x^3 - 31*x^2...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute binomial coefficient C(m,9).  The result is zero for m<9. */
static inline seq_t binom9(idx_t m)
{
    if (m < 9) return 0;
    seq_t r = 1;
    for (idx_t k = 1; k <= 9; ++k) {
        r = r * (seq_t)(m - 9 + k) / (seq_t)k;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* For n == 0 the offset is 1, but return 0 safety. */
    if (n == 0) return 0;

    /* Fixed coefficients p_i, i = 0..20. */
    const int coeff[21] = {
        0,          /* p0  */
        1,          /* p1  */
       -8,          /* p2  */
        31,         /* p3  */
       -66,         /* p4  */
        120,        /* p5  */
        48,         /* p6  */
       1326,        /* p7  */
       8185,        /* p8  */
      16945,        /* p9  */
      -8876,        /* p10 */
     -17425,        /* p11 */
      13363,        /* p12 */
       1487,        /* p13 */
      -2874,        /* p14 */
        329,        /* p15 */
       -31,         /* p16 */
        90,         /* p17 */
       -28,         /* p18 */
        0,          /* p19 */
       -2          /* p20 */
    };

    seq_t pos = 0;   /* sum of positive contributions */
    seq_t neg = 0;   /* sum of absolute values of negative contributions */

    idx_t limit = (n < 20) ? n : 20;
    for (idx_t i = 0; i <= limit; ++i) {
        int c = coeff[i];
        if (c == 0) continue;

        /* Compute binomial coefficient C(n - i + 9, 9). */
        idx_t m = n - i + 9;
        seq_t b = binom9(m);

        seq_t prod = b * (seq_t)(c > 0 ? c : -c);
        if (c > 0)
            pos += prod;
        else
            neg += prod;
    }

    /* The final result is positive, thus pos >= neg. */
    return pos - neg;
}
