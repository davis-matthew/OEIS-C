/*
 * A060254  Primes which are the sum of two consecutive composite numbers.
 * https://oeis.org/A060254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 5582, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A060254 – primes that are the sum of two consecutive composite numbers.
 * A prime p is in the sequence iff p is odd, prime, and both (p-1)/2
 * and (p+1)/2 are composite (i.e. not prime).  The function enumerates
 * odd primes in ascending order, keeps those satisfying the condition,
 * and caches the results so that calls can be in any order.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrime.h"

static seq_t *cache      = NULL;   /* cached terms */
static size_t cache_size  = 0;      /* number of cached terms */
static size_t cache_cap   = 0;      /* allocated capacity */

static inline void push_cache(seq_t val)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;            /* allocation failure – unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0 in OEIS convention */

    static idx_t candidate = 3;    /* start checking from the first odd prime */

    while (cache_size < n) {
        if (isPrime(candidate)) {
            idx_t half = (candidate - 1) / 2;   /* (p-1)/2 */
            /* Both halves must be composite (i.e. not prime). */
            if (!isPrime(half) && !isPrime(half + 1)) {
                push_cache((seq_t)candidate);
            }
        }
        candidate += 2;  /* advance to next odd number */
    }
    return cache[n - 1];
}
