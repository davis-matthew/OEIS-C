/*
 * A145613  Numerator of the polynomial A_l(x) = sum_{d=1..l-1} x^(l-d)/d for
 *          index l=2n+1 evaluated at x=3.
 * https://oeis.org/A145613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of the sum Σ_{d=1}^{2n} 3^{(2n+1)-d} / d (i.e.
 *            l = 2n+1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    idx_t l = 2 * n + 1;          /* l = 2n+1 */
    seq_t num = 0;               /* running numerator   */
    seq_t den = 1;               /* running denominator */
    seq_t pow = 3;               /* 3^k, where k starts at 1 */

    for (idx_t k = 1; k <= 2 * n; ++k) {
        seq_t term_den = (seq_t)(l - k);      /* d = l - k */
        seq_t new_num = num * term_den + pow * den;
        seq_t new_den = den * term_den;
        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
        pow *= (seq_t)3;                 /* next power of 3 */
    }

    return num;   /* numerator of the reduced sum */
}
