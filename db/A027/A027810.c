/*
 * A027810  a(n) = (n+1)*binomial(n+5, 5).
 * https://oeis.org/A027810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*binomial(n+5,5)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 86, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t k = 1; k <= 5; ++k) {
        seq_t num = n + 6 - k;   /* numerator: n+5, n+4, …, n+1 */
        seq_t den = k;           /* denominator: 1,2,3,4,5 */

        /* cancel common factors between res and den to keep numbers small */
        seq_t g1 = gcd(res, den);
        res /= g1;
        den /= g1;

        /* cancel remaining common factors between num and den */
        seq_t g2 = gcd(num, den);
        num /= g2;
        den /= g2;

        /* multiply and divide */
        res *= num;
        if (den != 1)
            res /= den; /* should be exact */
    }
    /* multiply by the extra (n+1) factor */
    res *= (n + 1);
    return res;
}
