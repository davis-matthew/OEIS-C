/*
 * A103803  Primes p such that both 2p - 15 and 2p + 15 are primes.
 * https://oeis.org/A103803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   this program finds p such that p, 2p-15 and 2p+15 are all
 *            primes. It iterates over odd candidates, checking primality via
 *            the utility function is_prime(uint64_t). The results are cached
 *            so that out‑of‑order calls run in amortised O(n) time
 * Range:     u8 n <= 21, u16 n <= 737, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* return 0 for invalid index, but behaviour below offset does not matter */
    if (n == 0) return 0;

    /* static state for memoisation */
    static seq_t *cache = NULL;        /* array of found primes */
    static idx_t cache_size = 0;      /* number of primes stored */
    static idx_t cache_cap = 0;       /* allocated capacity of cache     */
    static uint64_t p_search = 11;    /* next odd number to test         */
    static bool initialized = false; /* flag to initialise static state */

    /* initialise static state on first call */
    if (!initialized) {
        cache_cap = 128;                   /* arbitrary starting size   */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_size = 0;
        p_search = 11;
        initialized = true;
    }

    /* search until we have at least n terms */
    while (cache_size < n) {
        /* test candidate p_search */
        if (is_prime(p_search)) {
            uint64_t a1 = 2 * p_search - 15;
            uint64_t a2 = 2 * p_search + 15;
            if (is_prime(a1) && is_prime(a2)) {
                /* store found prime */
                if (cache_size >= cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_size] = (seq_t)p_search;
                cache_size++;
            }
        }
        p_search += 2; /* skip even numbers */
    }

    return cache[n - 1];
}
