/*
 * A038872  Primes congruent to {0, 1, 4} mod 5.
 * https://oeis.org/A038872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that p mod 5 is 0, 1, or 4
 *            (non‑nonn)
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static sieve data */
static unsigned char *sieve = NULL;   /* 1 = prime, 0 = composite */
static idx_t sieve_limit = 0;         /* largest number currently sieved */

/* Extend the sieve up to `limit`. */
static void sieve_up_to(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    /* Allocate new sieve; zero-initialised. */
    unsigned char *new_sieve = (unsigned char *)calloc((size_t)(limit + 1), sizeof(unsigned char));
    if (!new_sieve)
        return;                     /* memory failure – leave old sieve as is */

    /* Mark all >1 as prime initialising to 1. */
    for (idx_t i = 2; i <= limit; ++i)
        new_sieve[i] = 1;

    /* Classic sieve of Eratosthenes. */
    for (idx_t p = 2; p * p <= limit; ++p)
        if (new_sieve[p]) {
            for (idx_t m = p * p; m <= limit; m += p)
                new_sieve[m] = 0;
        }

    /* Replace the old sieve. */
    if (sieve)
        free(sieve);
    sieve = new_sieve;
    sieve_limit = limit;
}

/* Return the n-th term of A038872. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;             /* undefined; safety return */

    if (n == 1)
        return (seq_t)5;             /* first term explicitly */

    idx_t limit = 20 * n + 100;      /* generous upper bound for n up to 10,000 */
    if (limit < 5)
        limit = 5;

    sieve_up_to(limit);
    idx_t count = 0;

    for (idx_t p = 2; p <= limit; ++p) {
        if (!sieve[p])
            continue;
        if (p == 5 || (p % 5 == 1) || (p % 5 == 4)) {
            ++count;
            if (count == n)
                return (seq_t)p;
        }
    }

    /* If not enough found, double the search range and retry. */
    while (1) {
        limit *= 2;
        sieve_up_to(limit);
        count = 0;
        for (idx_t p = 2; p <= limit; ++p) {
            if (!sieve[p])
                continue;
            if (p == 5 || (p % 5 == 1) || (p % 5 == 4)) {
                ++count;
                if (count == n)
                    return (seq_t)p;
            }
        }
    }
}
