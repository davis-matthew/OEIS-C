/*
 * A205704  Positions of multiples of 8 in A204890 (differences of primes).
 * https://oeis.org/A205704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the position of the n‑th multiple of 8 in A204890, where
 *            A204890 is formed by listing, for each prime pk (starting with
 *            3), the differences pk‑pj for all earlier primes pj. The
 *            function generates A204890 on demand and records the indices of
 *            terms divisible by 8
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Static state shared between calls */
static seq_t *cache = NULL;      /* array of positions of multiples of 8   */
static idx_t cache_size = 0;     /* how many have been stored             */
static size_t cache_cap = 0;     /* allocated capacity                    */

static uint64_t *prime_list = NULL;   /* list of primes processed so far     */
static size_t primes_size = 0;        /* number of primes stored            */
static size_t primes_cap = 0;         /* capacity of prime_list array        */

static idx_t pos_global = 0;          /* current position in A204890 (1‑based) */
static idx_t next_prime_to_process = 1;  /* index of next prime to output      */
static uint64_t next_prime_candidate = 3;/* candidate for the next prime        */

/* Grow the cache array when needed */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap) ? cache_cap * 2 : 16;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Grow the prime list array when needed */
static void grow_primes(void)
{
    size_t new_cap = (primes_cap) ? primes_cap * 2 : 16;
    prime_list = (uint64_t*)realloc(prime_list, new_cap * sizeof(uint64_t));
    primes_cap = new_cap;
}

/* Return the n-th index a(n).  n >= 1.  Undefined for n == 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* guard against underflow */

    /* Initialise on first call */
    if (primes_cap == 0) {
        primes_cap = 16;
        prime_list = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
        prime_list[0] = 2;           /* first prime */
        primes_size = 1;
        next_prime_to_process = 1;   /* next column starts with prime 3 */
        next_prime_candidate = 3;
    }
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate sequence until we have n multiples of 8 */
    while (cache_size < n) {
        /* Ensure we have the prime needed for the next column */
        if ((size_t)next_prime_to_process >= primes_size) {
            uint64_t cand = next_prime_candidate;
            while (!is_prime(cand)) cand += 2;
            if (primes_size == primes_cap) grow_primes();
            prime_list[primes_size++] = cand;
            next_prime_candidate = cand + 2;
        }

        uint64_t pk = prime_list[(size_t)next_prime_to_process];

        /* Compute differences with all earlier primes */
        for (size_t i = 0; i < (size_t)next_prime_to_process; ++i) {
            uint64_t diff = pk - prime_list[i];
            ++pos_global;                          /* move to next position */
            if ((diff & 7ULL) == 0) {               /* divisible by 8? */
                if (cache_size == cache_cap) grow_cache();
                cache[cache_size++] = (seq_t)pos_global;
            }
        }
        ++next_prime_to_process;                   /* proceed to next column */
    }
    return cache[n - 1];
}
