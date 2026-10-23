/*
 * A141819  a(1)=1. a(n) = the smallest integer > a(n-1) that is coprime to
 *          the number of 1's in the binary representation of n.
 * https://oeis.org/A141819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 177, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; for n ≥ 2,  a(n) = the smallest integer > a(n‑1)
   that is coprime to the number of 1’s in the binary representation of n */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

/* Count the ones in the binary form of an idx_t.
   Uses the compiler builtin if available, otherwise a simple loop. */
static unsigned int popcount_u64(idx_t x) {
#if defined(__builtin_popcountll)
    return (unsigned int)__builtin_popcountll((uint64_t)x);
#else
    unsigned int c = 0;
    while (x) {
        c += (unsigned int)(x & 1);
        x >>= 1;
    }
    return c;
#endif
}

/* Static cache of previously computed terms.
   cache[i] stores a(i) for i ≥ 1; cache[0] is unused. */
static seq_t *cache = NULL;          /* array of terms */
static idx_t   cache_cap = 0;        /* number of allocated slots (≥ 0) */
static idx_t   cache_last = 0;       /* index of the largest term computed */

/* Return the n‑th term of the sequence.  The function is robust to
   calls with any order of n; values are computed and cached as
   necessary. */
seq_t a(idx_t n) {
    if (n < 1)                  /* offset 1 – undefined below 1; return 0 */
        return 0;

    /* Ensure the cache has room for index n. */
    if (cache_cap <= n) {
        idx_t new_cap = n + 1;                 /* include index 0 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                               /* allocation failed */
            return 0;
        cache = tmp;
        /* Zero the newly allocated part; old part stays intact. */
        for (idx_t i = cache_cap; i < new_cap; ++i)
            cache[i] = 0;
        cache_cap = new_cap;
    }

    /* Initialize a(1) if not yet done. */
    if (cache_last < 1) {
        cache[1] = (seq_t)1;
        cache_last = 1;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t cand = prev + 1;
        unsigned int ones = popcount_u64(i);
        seq_t ones_seq = (seq_t)ones;

        /* Find the smallest cand > prev that is coprime to the popcount. */
        while (gcd(cand, ones_seq) != (seq_t)1)
            cand++;

        cache[i] = cand;
    }
    cache_last = n;
    return cache[n];
}
