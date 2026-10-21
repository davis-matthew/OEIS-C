/*
 * A058274  An approximation to sigma_{5/2}(n): ceiling( sum_{d|n} d^(5/2) ).
 * https://oeis.org/A058274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil( Σ_{d|n} d^(5/2) )
 * Range:     u8 n <= 8, u16 n <= 77, u32 n <= 6419, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* safety – OEIS offset 1 */

    unsigned long long nn = (unsigned long long)n;
    long double sum = 0.0L;

    /* enumerate divisors in pairs (i, n/i) */
    for (unsigned long long i = 1; i <= nn / i; ++i) {
        if (nn % i == 0) {
            unsigned long long d1 = i;
            sum += (long double)d1 * d1 * sqrtl((long double)d1);

            unsigned long long d2 = nn / i;
            if (d2 != d1) {
                sum += (long double)d2 * d2 * sqrtl((long double)d2);
            }
        }
    }

    /* small epsilon to guard against floating‑point round‑off when sum
       is very close to an integer.  A very small value is enough.      */
    const long double eps = 1e-10L;

    unsigned long long res = (unsigned long long)ceill(sum - eps);
    return (seq_t)res;
}
