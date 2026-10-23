/*
 * A143348  a(n) = -(-1)^n times sum of divisors of n.
 * https://oeis.org/A143348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = -(-1)^n * σ(n) where σ(n) is the sum of divisors of n.
 *            a(1) = 1, a(2) = -3, a(3) = 4, a(4) = -7, …
 * Range:     i8 n <= 59, i16 n <= 9239, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute σ(p^e) = 1 + p + p^2 + … + p^e for prime p and exponent e. */
static inline seq_t sigma_prime_power(idx_t p, idx_t e)
{
    seq_t sum = 0;
    seq_t power = 1;          /* p^0 */
    for (idx_t i = 0; i <= e; ++i) {
        sum += power;
        power *= (seq_t)p;
    }
    return sum;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        abort();                  /* offset is 1 */

    idx_t rem = n;
    seq_t sigma = 1;

    for (idx_t p = 2; p <= rem / p; ++p) {
        if (rem % p == 0) {
            idx_t e = 0;
            while (rem % p == 0) {
                rem /= p;
                ++e;
            }
            sigma *= sigma_prime_power(p, e);
        }
    }
    if (rem > 1) {                   /* remaining prime factor */
        sigma *= sigma_prime_power(rem, 1);
    }

    seq_t sign = (n & 1) ? 1 : -1;   /* + for odd n, - for even n */
    return sigma * sign;
}
