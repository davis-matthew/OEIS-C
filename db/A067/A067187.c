/*
 * A067187  Numbers that can be expressed as the sum of two primes in exactly
 *          one way.
 * https://oeis.org/A067187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smallest integer that can be expressed as the
 *            sum of two primes in exactly one unordered way. The
 *            implementation uses a sieve of Eratosthenes to generate primes
 *            up to the current number being tested, counts the number of
 *            Goldbach partitions for each integer, stores each qualifying...
 * Range:     u8 n <= 57, u16 n <= 5002, u32 n <= 5002, u64 n <= 5002,
 *            u128 n <= 5002, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Prime sieve utilities ---------- */
static uint8_t *prime_sieve = NULL;        /* 1-byte flags: 1 = prime, 0 = composite */
static size_t sieve_limit = 0;             /* current largest number sieved */
static uint32_t *prime_array = NULL;       /* list of primes ≤ sieve_limit */
static size_t prime_count = 0;             /* number of primes in prime_array */

/* Build or rebuild the prime sieve up to at least 'limit'. */
static void rebuild_sieve(size_t limit)
{
    /* free previous data */
    free(prime_sieve);
    free(prime_array);

    /* allocate sieve */
    sieve_limit = limit + 1;                      /* +1 so that index = limit */
    prime_sieve = (uint8_t*)calloc(sieve_limit, sizeof(uint8_t));
    if (!prime_sieve) return;                    /* allocation failure – abort silently */

    /* Sieve of Eratosthenes */
    prime_sieve[0] = prime_sieve[1] = 0;
    for (size_t i = 2; i < sieve_limit; ++i) prime_sieve[i] = 1;
    for (size_t i = 2; i * i < sieve_limit; ++i) {
        if (prime_sieve[i]) {
            for (size_t j = i * i; j < sieve_limit; j += i)
                prime_sieve[j] = 0;
        }
    }

    /* collect primes into prime_array */
    /* first count them */
    size_t cnt = 0;
    for (size_t i = 2; i < sieve_limit; ++i)
        if (prime_sieve[i]) ++cnt;
    prime_array = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!prime_array) { prime_sieve = NULL; return; }
    size_t idx = 0;
    for (size_t i = 2; i < sieve_limit; ++i)
        if (prime_sieve[i]) prime_array[idx++] = (uint32_t)i;
    prime_count = cnt;
}

/* ---------- Global cache of sequence values ---------- */
static seq_t *term_cache = NULL;              /* array of already computed sequence terms */
static size_t term_len  = 0;                  /* number of terms stored so far */
static size_t term_cap  = 0;                  /* capacity of term_cache */

/* Helper: ensure that term_cache has at least 'need' capacity */
static void ensure_term_capacity(size_t need)
{
    if (need <= term_cap) return;
    size_t new_cap = term_cap ? term_cap << 1 : 1024;
    if (new_cap < need) new_cap = need;
    seq_t *new_cache = (seq_t*)realloc(term_cache, new_cap * sizeof(seq_t));
    if (!new_cache) return;          /* allocation failure – abort silently */
    term_cache = new_cache;
    term_cap   = new_cap;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0 – offset is 1 */

    /* Static variables that remember the current state across calls */
    static size_t next_to_test = 4;        /* next integer to test for membership */
    static size_t tested_up_to = 3;        /* highest integer already tested */

    /* Main loop: generate terms until we have at least n of them */
    while (term_len < (size_t)n) {
        /* ensure we have primes up to next_to_test */
        if (next_to_test > sieve_limit) {
            size_t new_limit = next_to_test * 2 + 100;   /* give some slack */
            rebuild_sieve(new_limit);
        }

        /* Count Goldbach partitions of 'next_to_test' */
        size_t cnt = 0;
        /* iterate over primes ≤ value/2 */
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p = prime_array[i];
            if (p > next_to_test / 2) break;
            if (prime_sieve[next_to_test - p]) {
                ++cnt;
                if (cnt > 1) break;        /* only need to know if >1 */
            }
        }

        /* If exactly one representation, store the term */
        if (cnt == 1) {
            ensure_term_capacity(term_len + 1);
            term_cache[term_len++] = (seq_t)next_to_test;
        }

        /* Update state for next iteration */
        ++next_to_test;
        tested_up_to = next_to_test - 1;
    }

    return term_cache[n - 1];
}
