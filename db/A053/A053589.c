/*
 * A053589  Greatest primorial number (A002110) which divides n.
 * https://oeis.org/A053589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2309, u16 n <= 2309, u32 n <= 2309, u64 n <= 2309,
 *            u128 n <= 2309, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  Formula: a(n) is the greatest primorial (product of the first k primes) that
 *  divides n.  It is obtained by multiplying the smallest primes p=2,3,5,…
 *  as long as p divides n.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* C function for OEIS sequence A053589 */
seq_t a(idx_t n)
{
    /* a(1) = 1 by definition; n >= 1 for the OEIS offset */
    if (n <= 1) return (seq_t)1;

    /* Prime table generated once, up to LIMIT = 10 000 (more than enough for n ≤ 10 000) */
    static const int LIMIT = 10000;
    static idx_t *primes = NULL;
    static size_t prime_count = 0;
    static bool initialized = false;

    if (!initialized) {
        /* Simple sieve of Eratosthenes */
        bool *sieve = (bool*)malloc((LIMIT + 1) * sizeof(bool));
        for (int i = 0; i <= LIMIT; ++i) sieve[i] = true;
        sieve[0] = sieve[1] = false;
        for (int i = 2; i * i <= LIMIT; ++i) {
            if (sieve[i]) {
                for (int j = i * i; j <= LIMIT; j += i) sieve[j] = false;
            }
        }
        /* Count primes first */
        for (int i = 2; i <= LIMIT; ++i)
            if (sieve[i]) ++prime_count;
        /* Allocate array and fill primes */
        primes = (idx_t*)malloc(prime_count * sizeof(idx_t));
        size_t idx = 0;
        for (int i = 2; i <= LIMIT; ++i)
            if (sieve[i]) primes[idx++] = (idx_t)i;
        free(sieve);
        initialized = true;
    }

    /* Compute the greatest primorial divisor of n */
    seq_t product = 1;
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = primes[i];
        if (n % p != 0) break;          /* prime does not divide n -> stop */
        product *= (seq_t)p;             /* multiply current prime once */
    }
    return product;
}
