/*
 * A143581  Numerators of coefficient of x^(n+1/2) in the series expansion of
 *          the haversine.
 * https://oeis.org/A143581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of 2 * binom(2n-2, n-1) / ( 4^(n-1) * (2n-1) )
 *            (reduced to lowest terms)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binom, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* not used, offset is 1 */

    /* indices for the binomial coefficient */
    idx_t k = n - 1;                           /* k = n-1 >= 0 */
    idx_t binom_n = 2 * k;                     /* 2k = 2n-2 */

    /* compute numerator: 2 * binom(2n-2, n-1) */
    seq_t num = binom(binom_n, k) << 1;        /* multiply by 2 */

    /* exponent of 2 in the numerator */
    idx_t v2 = 0;
    seq_t tmp = num;
    while ((tmp & 1) == 0) {                   /* keep shifting until odd */
        tmp >>= 1;
        v2++;
    }

    /* total power of 2 in the denominator (from 4^(n-1)) */
    idx_t den_exp = 2 * k;                      /* 2*(n-1) */

    /* how many twos we can cancel */
    idx_t t = (v2 < den_exp) ? v2 : den_exp;

    /* numerator after cancelling common powers of 2 */
    seq_t num_red = num >> t;

    /* odd part of the denominator */
    seq_t odd = (seq_t)(2 * n - 1);

    /* cancel any odd common factor */
    seq_t g = gcd(num_red, odd);

    return num_red / g;
}
