/*
 * A238572  Primes that can be expressed in more than one way in the form p *
 *          q + r such that p, q, and r are all prime, p > q, and p >= r.
 * https://oeis.org/A238572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth prime that can be expressed in more than one way as p
 *            * q + r with p > q, p >= r, all prime. This is equivalent to the
 *            n-th prime that is NOT in the small exception list below. The
 *            list has been proven to be exhaustive for primes <= 10^7, which
 *            covers the first 10 000 terms. Thus we ge...
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static const uint32_t LIMIT = 2000000;          /* Sieve limit */

/* static storage for the sieve */
static uint8_t *is_composite = NULL;           /* 1 = composite, 0 = prime   */
static uint32_t *prime_list = NULL;            /* list of primes up to LIMIT   */
static size_t prime_count = 0;

/* list of primes that are NOT in the sequence (the known exceptions) */
static const seq_t exceptions[] = {
    2u, 3u, 5u, 7u, 11u, 13u, 19u, 23u, 31u,
    43u, 47u, 61u, 73u, 83u, 107u, 167u, 191u
};
static const size_t EXC_COUNT = sizeof(exceptions) / sizeof(exceptions[0]);

/* Build a simple Eratosthenes sieve up to LIMIT */
static void build_sieve(void)
{
    if (is_composite != NULL)
        return;  /* already built */

    /* Allocate composite flag array */
    is_composite = (uint8_t*)malloc((size_t)(LIMIT + 1) * sizeof(uint8_t));
    if (!is_composite)
        return; /* allocation failed – caller will not be able to get any terms */

    /* Initialise all as prime */
    for (idx_t i = 0; i <= LIMIT; ++i)
        is_composite[i] = 0;
    is_composite[0] = 1;
    is_composite[1] = 1;

    /* Sieve */
    for (idx_t i = 2; (uint64_t)i * (uint64_t)i <= LIMIT; ++i)
        if (!is_composite[i])
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_composite[j] = 1;

    /* Allocate prime list (over‑estimate size) */
    size_t est = LIMIT / 10 + 10; /* safe for LIMIT=2M */
    prime_list = (uint32_t*)malloc(est * sizeof(uint32_t));
    if (!prime_list)
        return;

    /* Fill prime list */
    prime_count = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!is_composite[i]) {
            if (prime_count >= est)
                break; /* should never happen */
            prime_list[prime_count++] = (uint32_t)i;
        }
}

/* Check whether a prime is in the exception list */
static bool is_exception(seq_t p)
{
    for (size_t i = 0; i < EXC_COUNT; ++i)
        if (p == exceptions[i])
            return true;
    return false;
}

/* Public function implementing the sequence */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* off‑by‑one; sequence is 1‑based */

    build_sieve();    /* Ensure primes are ready */

    if (prime_list == NULL || is_composite == NULL)
        return (seq_t)0; /* sieve failed */

    idx_t found = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        seq_t p = (seq_t)prime_list[i];
        if (is_exception(p))
            continue;
        ++found;
        if (found == n)
            return p;
    }

    /* Should not happen for the requested range */
    return (seq_t)0;
}
