/*
 * A097706  Part of n composed of prime factors of form 4k+3.
 * https://oeis.org/A097706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product of the prime powers p^e of n for which the prime
 *            p satisfies p ≡ 3 (mod 4)
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset 1; n==0 is unused, safe return */

    seq_t result = 1;
    idx_t m = n;

    /* factor out 2 separately (2 mod 4 == 2, ignore it) */
    while ((m & 1) == 0)
        m >>= 1;

    /* trial divide by odd primes */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            /* count exponent of p */
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            if (p % 4 == 3) {
                /* multiply result by p^exp */
                seq_t pow = 1;
                for (idx_t i = 0; i < exp; ++i)
                    pow *= p;
                result *= pow;
            }
        }
    }

    /* residual prime factor (if any) */
    if (m > 1 && m % 4 == 3)
        result *= m;

    return result;
}
