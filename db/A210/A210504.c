/*
 * A210504  Numbers n for which 2*n+5, 4*n+5, 6*n+5, and 8*n+5 are primes.
 * https://oeis.org/A210504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer m such that 2*m+5, 4*m+5, 6*m+5, and
 *            8*m+5 are all prime. The sequence is indexed from 0
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 47, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Small primes used for a quick pre‑filter.
   They are larger than 3 and cover a wheel that eliminates
   the majority of composites early. */
static const uint16_t SMALL_PRIMES[] = {
    5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47
};
#define N_SMALL (sizeof(SMALL_PRIMES)/sizeof(SMALL_PRIMES[0]))

/* Cached terms of the sequence. */
static seq_t *cache = NULL;
static size_t cache_len = 0;      /* number of valid entries */
static size_t cache_cap = 0;      /* allocated capacity */

/* The next multiple of 3 to test.  All terms are multiples of 3. */
static seq_t current_candidate = 0;

/* Stored residues of current_candidate modulo the small primes.
   len == N_SMALL, residue[i] == current_candidate % SMALL_PRIMES[i]. */
static uint16_t residues[N_SMALL] = {0};

/* Ensure that cache contains at least (index+1) elements. */
static void ensure_cache(idx_t index)
{
    /* Grow the cache if necessary. */
    while (cache_cap < (size_t)(index + 1)) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* fatal: out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have enough. */
    while (cache_len <= (size_t)index) {
        seq_t n = current_candidate;

        /* Compute the four linear expressions. */
        seq_t v2 = 2 * n + 5;   /* 2*n+5 */
        seq_t v4 = 4 * n + 5;   /* 4*n+5 */
        seq_t v6 = 6 * n + 5;   /* 6*n+5 */
        seq_t v8 = 8 * n + 5;   /* 8*n+5 */

        /* Quick composite filter using the small primes. */
        bool bad = false;
        for (size_t i = 0; i < N_SMALL && !bad; ++i) {
            uint16_t p = SMALL_PRIMES[i];

            /* If a value equals the small prime, it is allowed. */
            if ((v2 % p == 0) && (v2 != (seq_t)p)) bad = true;
            else if ((v4 % p == 0) && (v4 != (seq_t)p)) bad = true;
            else if ((v6 % p == 0) && (v6 != (seq_t)p)) bad = true;
            else if ((v8 % p == 0) && (v8 != (seq_t)p)) bad = true;
        }

        /* If it passes the filter, test the four primes. */
        if (!bad) {
            if (is_prime_big(v2) && is_prime_big(v4) &&
                is_prime_big(v6) && is_prime_big(v8)) {
                /* Store the qualifying value. */
                cache[cache_len++] = n;
            }
        }

        /* Advance to the next candidate (next multiple of 3). */
        current_candidate += 3;

        /* Update residues for the next iteration. */
        for (size_t i = 0; i < N_SMALL; ++i) {
            residues[i] += 3;
            if (residues[i] >= SMALL_PRIMES[i]) residues[i] -= SMALL_PRIMES[i];
        }
    }
}

/* The exported sequence function. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
