/*
 * A219254  Number of ways to express 2n+1 as p+4q with p, q primes.
 * https://oeis.org/A219254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write 2*n+1 as p + 4*q with p and q
 *            both primes. The number of solutions is computed by iterating
 *            over all primes q ≤ (2*n+1-2)/4 and testing primality of p =
 *            2*n+1 - 4*q. The implementation uses a lazily‑expanded sieve
 *            that stores all primes up to the current maximum req...
 * Range:     u8 n <= 7296, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Lazy global data for the sieve and prime list. */
static idx_t sieve_limit = 0;      /* maximum index currently sieved */
static bool *sieve = NULL;         /* sieve[i] == true  → i is composite */
static uint64_t *prime_list = NULL;/* array of primes up to sieve_limit */
static size_t prime_count = 0;     /* number of primes stored */

/* Expand the sieve to cover all numbers ≤ limit.
   The routine rebuilds the entire sieve each time it is called;
   for the range required by this sequence (< 20001) this cost
   is negligible.  In C++ the result of malloc is cast. */
static void ensure_sieve(idx_t limit) {
    if (limit <= sieve_limit)
        return;

    size_t size = (size_t)(limit + 1);
    bool *new_sieve = (bool *)malloc(size * sizeof(bool));
    if (!new_sieve)
        exit(EXIT_FAILURE);

    /* initialise all as potentially prime */
    for (size_t i = 0; i < size; ++i)
        new_sieve[i] = false;
    if (size > 0)    new_sieve[0] = true; /* 0 is composite */
    if (size > 1)    new_sieve[1] = true; /* 1 is composite */

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; (idx_t)(i * i) <= limit; ++i) {
        if (!new_sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                new_sieve[j] = true;
        }
    }

    /* Count and collect the primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!new_sieve[i])
            ++cnt;

    uint64_t *new_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_list)
        exit(EXIT_FAILURE);

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!new_sieve[i])
            new_list[idx++] = (uint64_t)i;

    /* Free old data and install the new structures */
    free(sieve);
    free(prime_list);
    sieve = new_sieve;
    prime_list = new_list;
    prime_count = cnt;
    sieve_limit = limit;
}

/* Count the decompositions p + 4*q = 2*n+1 with p,q primes. */
seq_t a(idx_t n)
{
    /* 2*n+1 is the target odd number */
    idx_t target = 2 * n + 1;

    /* To test all candidates we need primes up to target. */
    ensure_sieve(target);
    if (target < 5)          /* 2*n+1 < 5 => no representation possible */
        return 0;

    /* q must satisfy 4*q ≤ target-2  →  q ≤ (target-2)/4 */
    idx_t max_q = (target - 2) / 4;

    seq_t count = 0;
    for (size_t i = 0; i < prime_count && prime_list[i] <= max_q; ++i) {
        idx_t q = prime_list[i];
        idx_t p = target - 4 * q;
        if (p >= 2 && is_prime((idx_t)p))
            ++count;
    }
    return count;
}
