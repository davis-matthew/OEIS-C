/*
 * A126974  a(1)=0. a(2)=1. a(n) = a(d(n)) + a(phi(n)), where d(n) =
 *          A000005(n), phi(n) = A000010(n).
 * https://oeis.org/A126974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 943, u16 n <= 9229, u32 n <= 9229, u64 n <= 9229,
 *            u128 n <= 9229, big any n
 * Helpers:   divisor_count_u64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A126974: a(1)=0, a(2)=1,
   a(n)=a(d(n))+a(phi(n)) where d(n)=number of divisors, phi(n)=Euler totient. */

#include <stdlib.h>
#include <string.h>
#include "../utils/phi.h"                 /* provides seq_t phi(idx_t) */
#include "../utils/divisor_count_u64.h"   /* provides uint64_t divisor_count_u64(uint64_t) */

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
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_size);
        if (!new_cache) return 0;                 /* out of memory: abort */

        /* Copy existing data */
        if (cache) memcpy(new_cache, cache, sizeof(seq_t) * cache_size);

        /* Initialise new entries to zero */
        memset(new_cache + cache_size, 0,
               sizeof(seq_t) * (new_size - cache_size));

        /* Free old array */
        if (cache) free(cache);

        cache      = new_cache;
        cache_size = new_size;
    }

    /* Return cached value if already computed (except for index 1) */
    if (cache[n] != 0) return cache[n];

    /* Compute the divisors count and Euler totient */
    idx_t d_val   = (idx_t)divisor_count_u64((uint64_t)n);
    idx_t phi_val = (idx_t)phi(n);

    /* Recursively compute a(d) and a(phi) */
    seq_t val = a(d_val) + a(phi_val);

    /* Store result in cache for future calls */
    cache[n] = val;
    return val;
}
