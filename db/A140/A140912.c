/*
 * A140912  Number of 3 X 8 matrices with elements in 0..n with each row and
 *          each column in nondecreasing order. 3,8,n can be permuted, see
 *          formula.
 * https://oeis.org/A140912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * binomial(n+10,n) * binomial(n+10,n+1) * *
 *            binomial(n+10,n+2) / ((n+10)^2 * (n+9))
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   binom, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/gcd.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t s = n + 10;                     /* s = n + 8 + 3 - 1               */

    /* binomial values */
    seq_t c0 = binom(s, n);               /* C(s,n)                           */
    seq_t c1 = binom(s, n + 1);           /* C(s,n+1)                         */
    seq_t c2 = binom(s, n + 2);           /* C(s,n+2)                         */

    /* numerator components: C(s,n), C(s,n+1), C(s,n+2), 2                */
    seq_t num[4] = { c0, c1, c2, (seq_t)2 };
    /* denominator components: s, s, (s-1)                                */
    seq_t den[3] = { (seq_t)s, (seq_t)s, (seq_t)(s - 1) };

    /* Reduce numerators and denominators by gcd to avoid overflow        */
    while (true) {
        bool changed = false;
        for (idx_t j = 0; j < 3; ++j) {
            for (idx_t i = 0; i < 4; ++i) {
                seq_t g = gcd(num[i], den[j]);
                if (g > 1) {
                    num[i] /= g;
                    den[j] /= g;
                    changed = true;
                }
            }
        }
        if (!changed)
            break;
    }

    /* Multiply remaining numerators – product equals the result            */
    seq_t result = 1;
    for (idx_t i = 0; i < 4; ++i)
        result *= num[i];

    return result;
}
