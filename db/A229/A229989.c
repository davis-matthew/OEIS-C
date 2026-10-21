/*
 * A229989  Number of primes in the interval [floor(n/2), floor(3n/2)].
 * https://oeis.org/A229989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1924, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A229989: number of primes in the interval [⌊n/2⌋, ⌊3n/2⌋] */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static size_t sieve_limit = 0;          /* highest value that has been sieved     */
static uint8_t *sieve = NULL;           /* 1 = prime, 0 = composite               */
static seq_t *pi_prefix = NULL;         /* prefix sums of primes up to i          */
static uint64_t *primes = NULL;         /* list of primes encountered             */
static size_t prime_count = 0;          /* current number of primes               */
static size_t prime_cap   = 0;          /* allocated capacity for primes          */

/* Extend the sieve to cover all integers <= limit */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    size_t old_lim = sieve_limit;
    size_t new_lim = (size_t)limit;          /* limit fits in size_t for n≤1e4            */

    /* Resize sieve array and mark new slots as prime */
    uint8_t *new_sieve = (uint8_t *)realloc(sieve, sizeof(uint8_t) * (new_lim + 1));
    if (new_sieve == NULL)
        abort();
    sieve = new_sieve;
    for (size_t i = old_lim + 1; i <= new_lim; ++i)
        sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    /* Mark composites in the new range using existing primes */
    for (size_t p_idx = 0; p_idx < prime_count; ++p_idx) {
        uint64_t p = primes[p_idx];
        uint64_t start = p * p;
        if (start <= old_lim) {
            start = ((old_lim + 1 + p - 1) / p) * p;
        }
        if (start > new_lim) continue;
        for (uint64_t j = start; j <= new_lim; j += p)
            sieve[j] = 0;
    }

    /* Append new primes to the primes list */
    size_t needed_cap = prime_count + (new_lim - old_lim) / 2 + 1;
    if (prime_cap < needed_cap) {
        size_t new_cap = prime_cap == 0 ? 16 : prime_cap * 2;
        while (new_cap < needed_cap) new_cap *= 2;
        uint64_t *new_primes = (uint64_t *)realloc(primes, sizeof(uint64_t) * new_cap);
        if (new_primes == NULL)
            abort();
        primes = new_primes;
        prime_cap = new_cap;
    }
    for (size_t i = old_lim + 1; i <= new_lim; ++i) {
        if (sieve[i])
            primes[prime_count++] = i;
    }

    /* Build/extend prefix array of prime counts */
    seq_t *new_pi = (seq_t *)realloc(pi_prefix, sizeof(seq_t) * (new_lim + 1));
    if (new_pi == NULL)
        abort();
    pi_prefix = new_pi;
    seq_t prev = old_lim == 0 ? 0 : pi_prefix[old_lim];
    for (size_t i = old_lim + 1; i <= new_lim; ++i) {
        prev += sieve[i] ? 1 : 0;
        pi_prefix[i] = prev;
    }

    sieve_limit = new_lim;
}

/* a(n) – number of primes in [⌊n/2⌋, ⌊3n/2⌋] */
seq_t a(idx_t n)
{
    idx_t lo = n / 2;                      /* floor(n/2)   */
    idx_t hi = (3 * n) / 2;                /* floor(3n/2) */

    ensure_sieve(hi);

    if (lo == 0)
        return pi_prefix[hi];
    else
        return pi_prefix[hi] - pi_prefix[lo - 1];
}
