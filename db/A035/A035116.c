/*
 * A035116  a(n) = tau(n)^2, where tau(n) = A000005(n).
 * https://oeis.org/A035116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = tau(n)^2, where tau(n) is the number of divisors of n
 * Range:     u8 n <= 119, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n>=1. For n==0 we return 0. */
    if (n == 0)
        return 0;

    uint64_t m = (uint64_t)n;   /* work with a mutable 64‑bit copy */
    uint64_t tau = 1;           /* product of (exponent+1) parts */
    uint64_t cnt, p;

    /* factor 2 separately */
    cnt = 0;
    while ((m & 1ULL) == 0) {
        cnt++;
        m >>= 1;
    }
    if (cnt > 0)
        tau *= (cnt + 1);

    /* odd prime factors */
    for (p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            cnt = 0;
            while (m % p == 0) {
                m /= p;
                cnt++;
            }
            tau *= (cnt + 1);
        }
    }

    /* if a prime factor larger than sqrt(n) remains */
    if (m > 1)
        tau *= 2;

    /* result is tau^2, cast to the output type */
    return (seq_t)(tau * tau);
}
