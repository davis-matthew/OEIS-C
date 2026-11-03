/*
 * A126974  a(1)=0. a(2)=1. a(n) = a(d(n)) + a(phi(n)), where d(n) =
 *          A000005(n), phi(n) = A000010(n).
 * https://oeis.org/A126974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 943, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 91 terms through n = 9229 (2026-11-03)
 * Helpers:   mem, num_divisors, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A126974: a(1)=0, a(2)=1,
   a(n)=a(d(n))+a(phi(n)) where d(n)=number of divisors, phi(n)=Euler totient. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/num_divisors.h"
#include "../utils/totient.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Static memoisation cache */
    static seq_t *cache = NULL;          /* array of computed values */
    static idx_t  cache_size = 0;        /* size of cache (indices 0..cache_size-1) */

    /* Handle base cases directly */
    if (n == 1) return 0;
    if (n == 2) return 1;

    /* Ensure cache is large enough for index n */
    if (n >= cache_size) {
        /* Grow cache to at least n+1 elements */
        idx_t new_size = n + 1;
        seq_t *new_cache = mem_new(seq_t, new_size);
        if (!new_cache) return 0;                 /* out of memory: abort */

        /* Copy existing data */
        if (cache) mem_copy(new_cache, cache, sizeof(seq_t) * cache_size);

        /* Initialise new entries to zero */
        mem_set(new_cache + cache_size, 0, sizeof(seq_t) * (new_size - cache_size));

        /* Free old array */
        if (cache) mem_free(cache);

        cache      = new_cache;
        cache_size = new_size;
    }

    /* Return cached value if already computed (except for index 1) */
    if (cache[n] != 0) return cache[n];

    /* Compute the divisors count and Euler totient */
    idx_t d_val   = (idx_t)num_divisors((uint64_t)n);
    idx_t phi_val = (idx_t)totient(n);

    /* Recursively compute a(d) and a(phi) */
    seq_t val = a(d_val) + a(phi_val);

    /* Store result in cache for future calls */
    cache[n] = val;
    return val;
}
