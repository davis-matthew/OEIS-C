/*
 * A120763  a(1) = 2. a(n) = a(n-1)*(largest prime occurring earlier in the
 *          sequence) - 1.
 * https://oeis.org/A120763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; for n>1, a(n) = a(n-1) * (largest prime occurring in
 *            a(1)..a(n-1)) – 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* 1‑based cache for previously computed terms */
static seq_t *cache = NULL;          /* stores a(i) at cache[i] */
static idx_t  cache_cap = 0;         /* allocated size of cache array */
static idx_t  cached_up_to = 0;      /* largest index computed so far */
static seq_t  largest_prime = (seq_t)0;   /* largest prime seen so far */

seq_t a(idx_t n)
{
    /* lazy initialisation */
    if (cache == NULL) {
        /* reserve space for indices 0 and 1 (index 0 unused) */
        cache_cap = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        /* a(1) = 2 */
        cache[1] = (seq_t)2;
        largest_prime = (seq_t)2;     /* 2 is the first prime */
        cached_up_to = 1;
    }

    /* extend the cache if a(n) is not yet computed */
    if (n > cached_up_to) {
        /* grow the array until its capacity exceeds n */
        if (cache_cap <= n) {
            size_t new_cap = cache_cap;
            while (new_cap <= (size_t)n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            cache = tmp;
            cache_cap = (idx_t)new_cap;
        }

        /* compute terms up to n */
        for (idx_t i = cached_up_to + 1; i <= n; ++i) {
            seq_t val = cache[i - 1] * largest_prime - (seq_t)1;
            cache[i] = val;

            /* update the largest prime if val is prime */
            if (is_prime_big(val) && val > largest_prime) {
                largest_prime = val;
            }
        }
        cached_up_to = n;
    }

    return cache[n];
}
