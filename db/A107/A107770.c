/*
 * A107770  Index of greater of twin primes in the primes.
 * https://oeis.org/A107770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = index of the greater of the n‑th twin prime pair.
 *            Computes this value by sieving primes, collecting twin indices,
 *            and extending the sieve until enough terms are available
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static state to keep results between calls */
static uint64_t sieve_limit    = 100;   /* current sieve upper bound          */
static size_t   twin_cnt       = 0;     /* number of twin indices found       */
static size_t   twin_cap       = 0;     /* capacity of twin_idx array         */
static uint32_t *twin_idx      = NULL;  /* array of twin indices (1‑based)    */

/* helper to ensure the twin_idx array has space for at least n elements */
static void ensure_twin_capacity(size_t n)
{
    if (n > twin_cap) {
        size_t new_cap = twin_cap ? twin_cap * 2 : 64;
        while (new_cap < n) new_cap *= 2;
        uint32_t *tmp = (uint32_t *)realloc(twin_idx, new_cap * sizeof(uint32_t));
        if (!tmp) abort();   /* memory exhausted */
        twin_idx = tmp;
        twin_cap = new_cap;
    }
}

/* rebuild the sieve up to 'limit' and collect twin prime indices.
   After this call, twin_cnt is set and twin_idx[0 .. twin_cnt-1] are filled. */
static void recompute_twin_indices(uint64_t limit)
{
    /* allocate composite flag array */
    uint8_t *comp = (uint8_t *)calloc(limit + 1, 1);
    if (!comp) abort();

    /* simple Eratosthenes sieve */
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (!comp[i]) {
            for (uint64_t j = i * i; j <= limit; j += i) comp[j] = 1;
        }
    }

    /* collect twin prime indices */
    twin_cnt = 0;
    ensure_twin_capacity(64);           /* initial allocation buffer */
    uint32_t prev_prime = 0;
    uint32_t prev_index = 0;
    uint32_t curr_index = 0;

    for (uint64_t i = 2; i <= limit; ++i) {
        if (!comp[i]) {
            curr_index++;                      /* 1‑based index of current prime */
            if (prev_prime && (i - prev_prime) == 2) {
                ensure_twin_capacity(twin_cnt + 1);
                twin_idx[twin_cnt++] = curr_index;
            }
            prev_prime   = (uint32_t)i;
            prev_index   = curr_index;
        }
    }

    free(comp);
    sieve_limit = limit;
}

/* the required sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* out of bounds handling, not used */

    /* enlarge sieve until we have at least n twin indices */
    while (twin_cnt < (size_t)n) {
        uint64_t new_limit = sieve_limit * 2;
        if (new_limit < sieve_limit + 1000) new_limit = sieve_limit + 1000; /* safety bump */
        recompute_twin_indices(new_limit);
    }

    /* return the n‑th twin index (values fit in 32‑bit for the problem limits) */
    return (seq_t)twin_idx[n - 1];
}
