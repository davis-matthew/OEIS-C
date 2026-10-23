/*
 * A127577  Numbers n for which 32n+31 is prime.
 * https://oeis.org/A127577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n for which 32*n+31 is prime (OEIS A127577)
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;   /* cached sequence values (restricted to seq_t)   */
static size_t capacity = 0;   /* current allocated capacity                     */
static size_t cache_size = 0; /* number of values actually stored               */
static idx_t next_n = 1;      /* next integer n to test for primality          */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safety for invalid index */

    /* initialise cache on first use */
    if (cache == NULL) {
        capacity = 64;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        cache_size = 0;
        next_n = 1;
    }

    /* extend the cache until it contains at least n terms */
    while (cache_size < n) {
        idx_t candidate = 32 * next_n + 31;   /* 32*n + 31 */
        if (is_prime(candidate)) {            /* test primality in idx_t space */
            if (cache_size >= capacity) {
                capacity *= 2;
                cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)next_n; /* store the n itself */
        }
        next_n++;
    }

    return cache[n - 1];
}
