/*
 * A028882  Numbers k such that k^2 - 7 is prime.
 * https://oeis.org/A028882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 21, u32 n <= 26, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for k such that k^2 - 7 is prime */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

#ifdef __cplusplus
# define SEQ_CONST(x) seq_t(x)
#else
# define SEQ_CONST(x) ((seq_t)(x))
#endif

seq_t a(idx_t n)
{
    /* Suppress warning for unused 0‑index */
    if (n == 0) return SEQ_CONST(0);

    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static idx_t  cache_size = 0;
    static seq_t  last_k = SEQ_CONST(0);

    /* First call: initialise the cache */
    if (cache == NULL)
    {
        cache_cap = 64;                  /* initial capacity – plenty for n≈10000 */
#ifdef __cplusplus
        cache = new seq_t[cache_cap];
#else
        cache = malloc(cache_cap * sizeof(seq_t));
#endif
        if (cache == NULL) exit(EXIT_FAILURE);
        cache[0] = SEQ_CONST(3);       /* first element */
        cache_size = 1;
        last_k = SEQ_CONST(3);
    }

    /* Return cached value if already computed */
    if (n - 1 < cache_size) return cache[n - 1];

    /* Extend cache until the desired index is reached */
    while (cache_size < n)
    {
        last_k = last_k + SEQ_CONST(1);
        seq_t val = last_k * last_k - SEQ_CONST(7);
        if (is_prime_big(val))
        {
            if (cache_size >= cache_cap)
            {
                size_t new_cap = cache_cap * 2;
#ifdef __cplusplus
                seq_t *new_cache = new seq_t[new_cap];
                for (size_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
                delete[] cache;
#else
                seq_t *new_cache = realloc(cache, new_cap * sizeof(seq_t));
                if (new_cache == NULL) exit(EXIT_FAILURE);
#endif
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_size++] = last_k;
        }
    }

    return cache[n - 1];
}
