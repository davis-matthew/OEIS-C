/*
 * A207637  Squarefree sums of 3 successive primes.
 * https://oeis.org/A207637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A207637: the nth squarefree sum of three successive primes. 
   The algorithm generates consecutive primes, tests each triple sum for 
   squarefreeness, and caches the results. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;           /* array of primes */
static size_t primes_cnt = 0;             /* number of primes in the array */
static idx_t prime_limit = 400000;        /* initial bound for prime generation */
static bool primes_ready = false;         /* have primes been built? */

/* cache of squarefree sums – zero‑based. cache[0] = a(1) */
static seq_t *cache = NULL;
static size_t cache_size = 0, cache_cap = 0;
static size_t next_triple = 0;            /* index of the next triple to test */

/* Build the prime list up to the current prime_limit if not yet done. */
static void ensure_primes(void)
{
    if (primes_ready) return;
    build_primes_upto(prime_limit, &primes, &primes_cnt);
    primes_ready = true;
}

/* Append one value to the cache, growing its capacity if needed. */
static void cache_append(seq_t value)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = value;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, but harmless */

    ensure_primes();

    while (cache_size < n) {
        /* If we run out of primes, enlarge the search limit. */
        if (next_triple + 2 >= primes_cnt) {
            prime_limit *= 2;
            free(primes);
            build_primes_upto(prime_limit, &primes, &primes_cnt);
            /* existing next_triple remains valid */
        }

        seq_t sum = (seq_t)primes[next_triple] +
                    (seq_t)primes[next_triple + 1] +
                    (seq_t)primes[next_triple + 2];

        if (is_squarefree(sum)) {
            cache_append(sum);
        }
        ++next_triple;
    }

    return cache[n - 1];
}
