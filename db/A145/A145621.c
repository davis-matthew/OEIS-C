/*
 * A145621  Numerator of the polynomial A_i(x) = Sum_{d=1..i-1} x^(i-d)/d for
 *          index i=2n+1 evaluated at x=7.
 * https://oeis.org/A145621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of sum_{d=1}^{2n} 7^{2n+1-d}/d
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. the numerator of the polynomial A_{2n+1}(7).      */
/* The implementation uses a simple rational sum, reducing after
   each term.  It works for any seq_t that supports the usual
   arithmetic operators and the gcd() routine from   */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1; a(1) corresponds to n=1. */
    if (n == 0) return 0;          /* unused for offset < 1 */

    const idx_t m = 2 * n;         /* m = 2n  */
    seq_t num = 0;                 /* current numerator   */
    seq_t den = 1;                 /* current denominator */

    seq_t pow = 1;                 /* will hold 7^e   */
    for (idx_t e = 1; e <= m; ++e) {
        pow = pow * (seq_t)7;      /* 7^e */

        const idx_t denom = m + 1 - e;   /* denominator = d */

        /* new fraction: num/den + pow/denom */
        seq_t new_num = num * denom + pow * den;
        seq_t new_den = den * denom;

        seq_t g = gcd(new_num, new_den);
        if (g > 1) {
            new_num /= g;
            new_den /= g;
        }

        num = new_num;
        den = new_den;
    }

    return num;   /* numerator of the reduced fraction */
}
