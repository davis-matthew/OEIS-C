/*
 * A156877  Number of primes <= n that are safe primes and also Sophie Germain
 *          primes.
 * https://oeis.org/A156877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes ≤ n that are both safe primes and also
 *            Sophie Germain primes
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Static cache for the primality sieve. */
static bool *sieve = NULL;
static idx_t sieve_limit = 0;

/* Ensure that the sieve is built up to at least 'limit'. */
static void ensure_sieve(idx_t limit)
{
    if (sieve_limit >= limit)
        return;                     /* already large enough */

    /* Allocate new sieve. */
    bool *new_sieve = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!new_sieve)
        return;                     /* allocation failed – callers will
                                        count zero in that rare case */

    /* Mark all as prime initially, then clear 0 and 1. */
    for (idx_t i = 0; i <= limit; ++i)
        new_sieve[i] = false;
    new_sieve[0] = true;
    if (limit >= 1)
        new_sieve[1] = true;

    /* Simple Eratosthenes. */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!new_sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                new_sieve[j] = true;
        }
    }

    /* Replace old sieve. */
    free(sieve);
    sieve = new_sieve;
    sieve_limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* outside the defined offset */

    /* We need to test primality up to 2*n+1 for 2p+1. */
    idx_t max_needed = 2 * n + 1;
    ensure_sieve(max_needed);
    if (!sieve)            /* allocation failure – return zero */
        return 0;

    seq_t count = 0;
    for (idx_t p = 2; p <= n; ++p) {
        if (!sieve[p]) {                     /* p is prime */
            idx_t q = (p - 1) / 2;
            if (q >= 2 && !sieve[q]) {       /* (p-1)/2 is also prime */
                idx_t pk = 2 * p + 1;
                if (pk <= max_needed && !sieve[pk]) { /* 2p+1 prime too */
                    count += 1;
                }
            }
        }
    }
    return count;
}
