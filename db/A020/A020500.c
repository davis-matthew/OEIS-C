/*
 * A020500  Cyclotomic polynomials at x=1.
 * https://oeis.org/A020500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = gcd(lpf(n), gpf(n)), where lpf is the least prime factor
 *            and gpf the greatest prime factor of n (a(1)=0)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)0;
    }

    /* least prime factor (lpf) */
    idx_t lpf = 0;
    for (idx_t i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            lpf = i;
            break;
        }
    }
    if (lpf == 0) {            /* n is prime */
        lpf = n;
    }

    /* greatest prime factor (gpf) */
    idx_t temp = n;
    idx_t gpf = 0;
    for (idx_t i = 2; i * i <= temp; ++i) {
        if (temp % i == 0) {
            gpf = i;          /* largest so far */
            while (temp % i == 0) {
                temp /= i;
            }
        }
    }
    if (temp > 1) {            /* remaining factor is prime */
        gpf = temp;
    }

    /* gcd of two primes is either the prime itself (when equal)
       or 1 */
    return (seq_t)(lpf == gpf ? lpf : (seq_t)1);
}
