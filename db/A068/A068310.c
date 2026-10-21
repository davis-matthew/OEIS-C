/*
 * A068310  n^2 - 1 divided by its largest square divisor.
 * https://oeis.org/A068310
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = squarefree part of n^2-1
 * Range:     u8 n <= 17, u16 n <= 257, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Sieve of Eratosthenes up to a fixed limit (sufficient for n ≤ 10000). */
    #define PRIME_LIMIT 20000
    static bool built = false;
    static bool sieve[PRIME_LIMIT + 1];
    static uint64_t primes[PRIME_LIMIT];
    static size_t prime_cnt = 0;

    if (!built) {
        /* Initialize the sieve array. */
        for (idx_t i = 0; i <= PRIME_LIMIT; ++i) sieve[i] = true;
        sieve[0] = sieve[1] = false;

        /* Mark non‑primes. */
        for (idx_t i = 2; i * i <= PRIME_LIMIT; ++i) {
            if (sieve[i]) {
                for (idx_t j = i * i; j <= PRIME_LIMIT; j += i)
                    sieve[j] = false;
            }
        }

        /* Store primes in the primes array. */
        for (idx_t i = 2; i <= PRIME_LIMIT; ++i)
            if (sieve[i])
                primes[prime_cnt++] = (uint64_t)i;

        built = true;
    }

    /* Compute n^2 – 1 using 64‑bit arithmetic. */
    uint64_t n64 = (uint64_t)n;
    uint64_t m   = n64 * n64 - 1;

    /* Remove all square factors p² dividing m. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t p2 = p * p;
        if (p2 > m) break;
        while (m % p2 == 0) m /= p2;
    }

    /* Return the squarefree part (truncated to seq_t). */
    return (seq_t)m;
}
