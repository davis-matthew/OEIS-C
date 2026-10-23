/*
 * A131134  a(1)=1. a(n) = (1/n + 1/a(n-1)) * lcm(n,a(n-1)).
 * https://oeis.org/A131134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>1, a(n) = (n + a(n-1)) / gcd(n, a(n-1))
 * Range:     u8 n <= 71, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed_max = 0;

    if (n == 0)            /* offset is 1; n==0 is unused */
        return (seq_t)0;

    /* Allocate or enlarge cache on first call or when needed */
    if (cache == NULL) {
        cache_size = n + 1;                     /* index 1..n */
        cache = (seq_t *)calloc((size_t)cache_size, sizeof(seq_t));
        if (cache == NULL)                      /* allocation failure */
            return (seq_t)0;
        cache[1] = (seq_t)1;
        computed_max = 1;
    }
    if (n >= cache_size) {                      /* grow the cache */
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (tmp != NULL) {
            /* zero the newly allocated part */
            size_t add = (size_t)(new_size - cache_size);
            seq_t *start = tmp + cache_size;
            for (size_t i = 0; i < add; ++i)
                start[i] = 0;
            cache = tmp;
            cache_size = new_size;
        }
    }

    /* If already computed, just return */
    if (n <= computed_max)
        return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t g = gcd((seq_t)i, prev);
        cache[i] = ((seq_t)i + prev) / g;
    }
    computed_max = n;
    return cache[n];
}
