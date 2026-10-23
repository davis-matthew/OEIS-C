/*
 * A166581  Primes containing the string 333.
 * https://oeis.org/A166581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   primes that contain the decimal substring "333" (OEIS A166581)
 * Range:     u8 none, u16 n <= 31, u32 n <= 34, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Utility: check whether x contains the substring 333 */
static inline bool has_333(idx_t x)
{
    unsigned cnt = 0;
    while (x > 0) {
        if (x % 10 == 3) {
            cnt++;
            if (cnt == 3) return true;
        } else {
            cnt = 0;
        }
        x /= 10;
    }
    return false;
}

/* Cache for computed terms */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;     /* number of allocated elements (indexable up to cache_cap-1) */
static idx_t  cache_max  = 0;     /* highest index that has been computed (1‑based) */

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1 – n=0 is outside the OEIS definition */

    /* Expand cache if necessary */
    if (cache_cap < (size_t)n + 1) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;           /* allocation failure – return 0 */
        /* Zero the newly added part to keep cache[0]=0 invariant */
        for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = 0;
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Return cached value if already computed */
    if (n <= cache_max) return cache[n];

    /* Compute missing terms up to n */
    idx_t candidate = (cache_max == 0) ? 2 : ((idx_t)cache[cache_max]) + 1;  /* start after last prime */
    while (cache_max < n) {
        if (has_333(candidate) && is_prime_big((seq_t)candidate)) {
            cache[++cache_max] = (seq_t)candidate;
        }
        ++candidate;
    }

    return cache[n];
}
