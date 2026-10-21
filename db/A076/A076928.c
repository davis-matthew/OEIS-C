/*
 * A076928  a(1) = 1, a(n+1)= a(n)*(n+1) divided by the largest prime divisor
 *          of n+1.
 * https://oeis.org/A076928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) * n / LP(n), a(1) = 1, where LP(n) is the largest
 *            prime divisor of n
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 26, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required headers */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/largest_prime_divisor.h"

/* static cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Return the nth term of OEIS A076928 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined, return 0 */
        return 0;

    /* extend cache up to n, if necessary */
    if ((size_t)n > cache_size) {
        size_t new_size = (size_t)n;
        seq_t *new_cache = NULL;

        if (cache == NULL) {
            /* first allocation: store a(1) = 1 */
            new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
            if (!new_cache && new_size > 0)
                abort();   /* out of memory */
            new_cache[0] = (seq_t)1;
            /* compute remaining terms if any */
            for (size_t i = 1; i < new_size; ++i) {
                uint64_t idx = i + 1;               /* current n */
                uint64_t lpd = largest_prime_divisor(idx);
                seq_t prod = new_cache[i - 1] * (seq_t)idx;
                new_cache[i] = prod / (seq_t)lpd;
            }
        } else {
            /* reallocate and compute new terms */
            new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (!new_cache && new_size > 0)
                abort();   /* out of memory */
            for (size_t i = cache_size; i < new_size; ++i) {
                uint64_t idx = i + 1;               /* current n */
                uint64_t lpd = largest_prime_divisor(idx);
                seq_t prod = new_cache[i - 1] * (seq_t)idx;
                new_cache[i] = prod / (seq_t)lpd;
            }
        }

        cache = new_cache;
        cache_size = new_size;
    }

    return cache[(size_t)n - 1];
}
