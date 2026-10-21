/*
 * A075254  a(n) = n + (sum of prime factors of n taken with repetition).
 * https://oeis.org/A075254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n + sum of prime factors of n with multiplicity
 *            (sopfr(n))
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A075254.  Uses trial division up to sqrt(n). */
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t m = n;

    /* factor out 2's */
    while ((m & 1ULL) == 0) {            /* m % 2 == 0 */
        sum += (seq_t)2;
        m >>= 1;                         /* m /= 2 */
    }

    /* factor odd primes */
    for (idx_t p = 3; p <= m / p; p += 2) {
        while (m % p == 0) {
            sum += (seq_t)p;
            m /= p;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        sum += (seq_t)m;
    }

    return (seq_t)n + sum;
}
