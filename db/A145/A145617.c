/*
 * A145617  Numerator of the polynomial A_l(x) = sum_{d=1..l-1} x^(l-d)/d for
 *          index l=2n+1 evaluated at x=5.
 * https://oeis.org/A145617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of Σ_{d=1}^{2n} 5^{2n+1-d} / d in lowest terms
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* l = 2*n + 1  (odd index) */
    idx_t l = (idx_t)2 * n + 1;

    seq_t num = 0;   /* current numerator */
    seq_t den = 1;   /* current denominator */

    /* pow holds 5^{l-d}; start with d = l-1 → exponent = 1 */
    seq_t pow = 5;

    for (idx_t d = l - 1; ; d--)
    {
        /* add term = pow / d to the running fraction */
        seq_t new_num = num * (seq_t)d + den * pow;
        seq_t new_den = den * (seq_t)d;
        seq_t g = seq_gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;

        if (d == 1)  /* last term processed */
            break;

        /* Prepare next power for the smaller d */
        pow *= 5;
    }

    return num;
}
