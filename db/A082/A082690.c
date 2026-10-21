/*
 * A082690  Denominator of n*sum(k=1,(-1)^(k+1)/(n+k)).
 * https://oeis.org/A082690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = denominator of n * sum_{k=1}^n (-1)^{k+1}/(n+k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t num = 0;   /* current numerator of the partial sum */
    seq_t den = 1;   /* current denominator of the partial sum */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t d = (seq_t)(n + k);           /* term denominator */
        seq_t mul = num * d;                /* cross‑multiplied numerator */

        seq_t new_num;
        if (k & 1U) { /* odd k → positive term */
            new_num = mul + den;
        } else {      /* even k → negative term */
            new_num = mul - den;           /* mul >= den holds for all n ≥ 1 */
        }

        seq_t new_den = den * d;
        seq_t g = gcd(new_num, new_den);   /* simplify */

        num = new_num / g;
        den = new_den / g;
    }

    /* multiply the final fraction by n and reduce again */
    seq_t mult = num * (seq_t)n;
    seq_t g = gcd(mult, den);
    den /= g;

    return den;      /* denominator in lowest terms */
}
