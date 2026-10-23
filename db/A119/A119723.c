/*
 * A119723  Numerator of Sum[ (-1)^(k-1) * 1/(Prime[k]-1)^2, {k,1,n}].
 * https://oeis.org/A119723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of Sum[ (-1)^(k-1) * 1/(Prime[k]-1)^2, {k,1,n}]
 *            evaluated in lowest terms. The function returns a(n)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t num = 0;      /* numerator of the running sum   */
    seq_t den = 1;      /* denominator of the running sum */

    for (idx_t k = 1; k <= n; ++k) {
        /* kth prime (1‑based) */
        uint32_t p = get_prime(k);                 /* prime[k]   */
        seq_t d = (seq_t)(p - 1);                  /* d = prime-1 */
        d = d * d;                                 /* d = (prime-1)^2 */

        /* sign for this term: + for odd k, - for even k */
        int sign = (k & 1U) ? 1 : -1;

        /* Compute gcd and lcm parts to avoid overflow */
        seq_t g = gcd(den, d);          /* gcd(den, d)   */
        seq_t mult = d / g;             /* factor for current numerator */
        seq_t term = den / g;           /* factor for the added term   */

        /* Update numerator: new_num = num * mult ± term */
        seq_t new_num = num * mult;
        if (sign > 0)
            new_num += term;
        else
            new_num -= term;           /* guaranteed non‑negative */

        /* Update denominator: new_den = (den / g) * d */
        seq_t new_den = term * d;       /* term = den / g */

        /* Reduce fraction to lowest terms again */
        seq_t g2 = gcd(new_num, new_den);
        if (g2 != 1) {
            new_num /= g2;
            new_den /= g2;
        }

        /* Prepare for next iteration */
        num = new_num;
        den = new_den;
    }

    return num;
}
