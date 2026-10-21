/*
 * A075699  Number of primes in the interval (n,4n].
 * https://oeis.org/A075699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = π(4n) – π(n) (number of primes in (n,4n])
 * Range:     u8 n <= 634, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds a sieve on demand and keeps a prefix sum
   array of the prime count.  It works for any seq_t and idx_t
   compatible with the build, including arbitrary‑precision seq_t. */

#include <stdlib.h>
#include <stdbool.h>

static idx_t sieve_limit = 0;            /* highest number currently sieved */
static bool *is_prime = NULL;            /* boolean array: is_prime[i] */
static seq_t *prime_count = NULL;        /* prefix sums: prime_count[i] = π(i) */

/* Expand the sieve up to 'limit' (inclusive) */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;               /* already sufficient */

    /* Allocate new arrays of the required size */
    bool *new_is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!new_is_prime)
        exit(EXIT_FAILURE);
    seq_t *new_prime_count = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    if (!new_prime_count)
        exit(EXIT_FAILURE);

    /* Assume all numbers >= 2 are prime initially */
    for (idx_t i = 0; i <= limit; ++i)
        new_is_prime[i] = true;
    if (limit >= 0) new_is_prime[0] = false;
    if (limit >= 1) new_is_prime[1] = false;

    /* Classic sieve: iterate p up to sqrt(limit) safely */
    for (idx_t p = 2; p <= limit / p; ++p) {
        if (new_is_prime[p]) {
            /* start marking from p*p; use 128‑bit to avoid overflow */
            __int128 start128 = (__int128)p * p;
            idx_t start = (idx_t)start128;
            for (idx_t m = start; m <= limit; m += p)
                new_is_prime[m] = false;
        }
    }

    /* Build prefix sums of primes */
    new_prime_count[0] = 0;
    for (idx_t i = 1; i <= limit; ++i)
        new_prime_count[i] = new_prime_count[i - 1] + (new_is_prime[i] ? 1 : 0);

    /* Replace old data */
    free(is_prime);
    free(prime_count);
    is_prime      = new_is_prime;
    prime_count   = new_prime_count;
    sieve_limit   = limit;
}

/* Number of primes in (n,4n] for n ≥ 1 */
seq_t a(idx_t n)
{
    if (n < 1)                /* undefined, return 0 for safety */
        return 0;

    idx_t upper = n * 4;      /* 4n - fits into idx_t for the intended range */
    ensure_sieve(upper);
    return prime_count[upper] - prime_count[n];
}
