/*
 * A168166  Primes which are the concatenation of a prime p and 2p+1.
 * https://oeis.org/A168166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n‑th prime q where q = concat(p, 2p+1) with p prime
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 6, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   concat_parts, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/concat_parts.h"
#include "../utils/is_prime_big.h"

static size_t prime_count = 0;          /* number of primes currently stored   */
static size_t prime_cap   = 0;          /* capacity of prime array              */
static uint32_t *prime_list = NULL;     /* array of primes (uint32_t is enough for our sieve) */

static idx_t sieve_limit = 0;           /* highest number sieved so far         */

static size_t cache_count = 0;          /* number of terms stored in cache     */
static size_t cache_cap   = 0;          /* capacity of cache array             */
static seq_t *cache = NULL;             /* array of computed terms             */

static idx_t processed_prime_index = 0; /* index of next prime to examine      */

/* ------------------------------------------------------------------------ */
/*  expand the list of primes up to 'limit' (inclusive) using a simple      */
/*  Eratosthenes sieve.  This replaces any previously stored primes.        */
static void expand_primes_to(idx_t limit) {
    /* Allocate a bit array for the sieve */
    size_t size = (size_t)limit + 1;
    bool *is_prime = (bool *)malloc(size);
    if (!is_prime) exit(1);
    for (size_t i = 0; i < size; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= (size_t)limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes and allocate the prime list */
    size_t new_count = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (is_prime[i]) ++new_count;

    if (prime_cap < new_count) {
        uint32_t *new_ptr = (uint32_t *)realloc(prime_list, new_count * sizeof(uint32_t));
        if (!new_ptr) exit(1);
        prime_list = new_ptr;
        prime_cap  = new_count;
    }

    /* Fill the list */
    size_t pos = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i) {
        if (is_prime[i]) prime_list[pos++] = (uint32_t)i;
    }
    prime_count = new_count;
    sieve_limit = limit;
    free(is_prime);
}

/* ------------------------------------------------------------------------ */
/*  append a new term to the cache, expanding the cache if necessary      */
static void cache_append(seq_t val) {
    if (cache_count == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) exit(1);
        cache = new_ptr;
        cache_cap = new_cap;
    }
    cache[cache_count++] = val;
}

/* ------------------------------------------------------------------------ */
/*  Main sequence function                                                */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n=0, return 0 as placeholder */

    /* Ensure we have at least one prime in the list */
    if (prime_count == 0) {
        expand_primes_to(2000000);   /* start with a reasonable limit */
        processed_prime_index = 0;
    }

    /* Generate terms until we have the n-th one */
    while (cache_count < n) {
        /* Process all currently known primes */
        for (; processed_prime_index < prime_count && cache_count < n; ++processed_prime_index) {
            idx_t p = (idx_t)prime_list[processed_prime_index];
            seq_t concat = concat_parts(p, 2 * p + 1);
            if (is_prime_big(concat))
                cache_append(concat);
        }

        /* If still not enough terms, extend the sieve */
        if (cache_count < n) {
            idx_t new_limit = sieve_limit * 2;
            if (new_limit == 0) new_limit = 2000000;
            expand_primes_to(new_limit);
            /* processed_prime_index already points to next unexamined prime */
        }
    }

    return cache[n - 1];
}
