/*
 * A097988  a(n) = Sum_{d dividing n} tau(d)^3 = (Sum_{d dividing n}
 *          tau(d))^2.
 * https://oeis.org/A097988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = (Sum_{d|n} tau(d))^2 = (Π_{p^e||n} (e+1)(e+2)/2)^2
 * Range:     u8 n <= 11, u16 n <= 719, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;           /* a(1) = 1 */

    idx_t x = n;
    seq_t prod = 1;                 /* Π (e+1)(e+2)/2 */

    /* handle factor 2 separately */
    idx_t e = 0;
    while ((x & 1ULL) == 0) {
        e++;
        x >>= 1;
    }
    if (e > 0) {
        seq_t term = (seq_t)(e + 1) * (e + 2) / 2;
        prod *= term;
    }

    /* odd primes up to sqrt(x) */
    for (idx_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            e = 0;
            while (x % p == 0) {
                e++;
                x /= p;
            }
            seq_t term = (seq_t)(e + 1) * (e + 2) / 2;
            prod *= term;
        }
    }

    /* remaining prime factor, if any */
    if (x > 1) {
        /* exponent e = 1 */
        seq_t term = (seq_t)(1 + 1) * (1 + 2) / 2; /* 3 */
        prod *= term;
    }

    return prod * prod;
}
