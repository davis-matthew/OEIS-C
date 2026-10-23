/*
 * A153762  Numbers k such that 8k + 9 is prime.
 * https://oeis.org/A153762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine returns the n-th integer k such that 8·k + 9 is prime
   (OEIS A153762).  The sequence is produced by a simple search:
   each candidate k is tested for primality of 8·k+9 and accepted if
   prime.  Results are memoised in a dynamically growing cache,
   so that calls with arbitrary n (not necessarily in increasing
   order) are served quickly.  The algorithm runs in O(n) time for
   n up to about 10 000 under the given limits.  */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;          /* dynamic cache of found ks */
static size_t cache_capacity = 0;    /* allocated size of cache     */
static size_t cache_size = 0;        /* number of entries in cache  */
static idx_t next_candidate = 0;     /* smallest k not yet tested   */

/* 
   Grow the cache to accommodate at least new_cap elements.
   Uses realloc; on failure the program aborts.
*/
static void ensure_cache_capacity(size_t new_cap)
{
    if (new_cap <= cache_capacity) return;
    size_t new_size = cache_capacity ? cache_capacity * 2 : 16;
    if (new_size < new_cap) new_size = new_cap;
    seq_t *new_cache = (seq_t *)realloc(cache,
                                         new_size * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed – abort; this situation should not occur
           for the intended usage. */
        abort();
    }
    cache = new_cache;
    cache_capacity = new_size;
}

seq_t a(idx_t n)
{
    /* The OEIS offset is 1.  Any request with n<1 is ignored. */
    if (n < 1) return 0;

    /* Ensure we have at least n entries in the cache. */
    while ((size_t)n > cache_size) {
        idx_t candidate = next_candidate;
        idx_t p = (idx_t)8 * candidate + (idx_t)9;
        if (is_prime(p)) {
            ensure_cache_capacity(cache_size + 1);
            cache[cache_size++] = (seq_t)candidate;
        }
        /* Prepare for the next candidate. */
        next_candidate = candidate + (idx_t)1;
    }

    return cache[n - 1];
}
