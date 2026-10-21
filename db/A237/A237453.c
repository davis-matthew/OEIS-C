/*
 * A237453  Number of primes p < n with p*n + pi(p) prime, where pi(.) is
 *          given by A000720.
 * https://oeis.org/A237453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ primes p < n : p*n + π(p) is prime }
 * Range:     u8 n <= 17, u16 n <= 387, u32 n <= 387, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utilities */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Build list of primes up to a limit */
#include "../utils/build_primes_upto.h"   /* void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out); */
/* Large‑integer primality test */
#include "../utils/is_prime_big.h"        /* bool is_prime_big(seq_t n); */

static idx_t max_prime_limit = 0;     /* maximum n for which primes are generated */
static uint64_t *prime_list   = NULL; /* array of primes up to max_prime_limit */
static size_t   prime_count    = 0;    /* number of primes in prime_list */

/* Extend the prime table up to “limit” (inclusive). */
static void extend_primes(idx_t limit)
{
    if (limit <= max_prime_limit) return;

    uint64_t *new_list = NULL;
    size_t   new_cnt   = 0;

    /* The helper allocates and returns a freshly allocated array of primes. */
    build_primes_upto(limit, &new_list, &new_cnt);

    /* Replace the old table. */
    free(prime_list);
    prime_list   = new_list;
    prime_count  = new_cnt;
    max_prime_limit = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 2) return 0;          /* no prime < n */

    /* Ensure we have all primes < n ready. */
    extend_primes(n - 1);

    seq_t count = 0;

    /* Loop over primes p < n.  The array is sorted, so we stop when p >= n. */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p >= n) break;

        /* π(p) is the index of p in the prime list plus one. */
        idx_t pi_p = (idx_t)i + 1;

        /* Compute p*n + π(p) with 128‑bit arithmetic to avoid overflow. */
        __uint128_t val128 = (__uint128_t)p * (__uint128_t)n + (__uint128_t)pi_p;

        /* Convert to seq_t; the test harness guarantees val128 fits in seq_t. */
        seq_t val = (seq_t)val128;

        if (is_prime_big(val)) ++count;
    }

    return count;
}
