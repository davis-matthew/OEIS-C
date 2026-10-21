/*
 * A083242  For n >= 3, a(n-3) + a(n-2) + a(n-1) + a(n) = prime(n); a(0) = 0,
 *          a(1) = 1, a(2) = 1.
 * https://oeis.org/A083242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 150, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_cache, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A083242
    a(n-3) + a(n-2) + a(n-1) + a(n) = prime(n)
    a(0) = 0, a(1) = 1, a(2) = 1
    prime(k) is the k‑th prime (prime(1)=2) */

/*  Implementation notes:
    - Uses static memoisation: cache is grown as needed.
    - get_prime() is supplied by the build and returns the k‑th prime.
    - ensure_cache() guarantees that the cache array can hold indices
      up to the given value.
    - All arithmetic is performed in seq_t; indices are safely cast to
      seq_t where required.                                         */

#include <stddef.h>                     /* for size_t */
#include "../utils/get_prime.h"          /* uint32_t get_prime(idx_t k) */
#include "../utils/ensure_cache.h"       /* void ensure_cache(idx_t, seq_t **, size_t *) */

seq_t a(idx_t n)
{
    /* Static cache holding computed terms.  Elements 0..last are valid. */
    static seq_t  *cache  = NULL;   /* pointer to cache array */
    static size_t  cap     = 0;      /* capacity of cache in elements */
    static idx_t   last    = 0;      /* highest index that has been computed */

    /* First call: allocate space for at least indices 0..2 and
       initialise base cases.  The sequence is defined for n >= 0.     */
    if (cache == NULL) {
        /* Needed capacity for indices 0,1,2 */
        ensure_cache(2, &cache, &cap);
        cache[0]   = (seq_t)0;
        cache[1]   = (seq_t)1;
        cache[2]   = (seq_t)1;
        last = 2;                     /* we have computed up to index 2 */
    }

    /* If the requested value is already known, return it immediately. */
    if (n <= last) {
        return cache[n];
    }

    /* Ensure the cache can hold the index n. */
    ensure_cache(n, &cache, &cap);

    /* Compute new terms up to index n inclusively. */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t sum   = cache[i - 3] + cache[i - 2] + cache[i - 1];
        seq_t prime = (seq_t)get_prime(i);      /* i is already 1‑based for primes */
        cache[i]    = prime - sum;
    }

    /* Update the furthest computed index. */
    last = n;

    return cache[n];
}
