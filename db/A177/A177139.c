/*
 * A177139  Numbers k such that (3*k-4, 3*k-2) is a twin prime pair.
 * https://oeis.org/A177139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) is the n-th k such that (3*k-4 , 3*k-2) form a twin prime
 *            pair. Computation: incremental search with caching
 * Range:     u8 n <= 16, u16 n <= 999, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the implementation relies on the helper function
   bool is_prime_small(seq_t num) and the generic realloc helper
   seq_t *seq_realloc(seq_t *old, size_t old_cap, size_t new_cap). */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"
#include "../utils/seq_realloc.h"

/* static cache of found k-values */
static seq_t *cache      = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
/* the next k value to test */
static idx_t next_k = 1;

seq_t a(idx_t n)
{
    /* offset 1 – for n==0 simply return 0 (unused by OEIS) */
    if (n == 0) return (seq_t)0;

    /* generate k's until we have at least n entries */
    while (cache_size < n) {
        idx_t k  = next_k++;
        idx_t p1 = 3 * k - 4;
        idx_t p2 = 3 * k - 2;

        /* test twin-primality */
        if (is_prime_small((seq_t)p1) && is_prime_small((seq_t)p2)) {
            /* grow cache if necessary */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) exit(1);           /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)k;
        }
    }
    return cache[n - 1];
}
