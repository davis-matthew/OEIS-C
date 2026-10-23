/*
 * A170819  a(n) = product of distinct primes of the form 4k-1 that divide n.
 * https://oeis.org/A170819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product of distinct primes of the form 4k-1 that divide n
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return 1;                /* a(1) = 1, a(0) not used */

    uint64_t m = (uint64_t)n;            /* work in 64‑bit integers */
    seq_t prod = 1;                      /* multiplicative accumulator */

    /* factor 2 – not needed for 4k-1 primes, so just remove it */
    if (m % 2 == 0)
        while (m % 2 == 0) m /= 2;

    /* trial division by odd numbers */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            if (p % 4 == 3)                /* 4k-1 prime found */
                prod *= (seq_t)p;
            while (m % p == 0)             /* strip all copies */
                m /= p;
        }
    }

    /* remaining factor > sqrt(original) */
    if (m > 1 && m % 4 == 3)
        prod *= (seq_t)m;

    return prod;
}
