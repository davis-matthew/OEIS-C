/*
 * A112278  a(0) = 1, a(1) = 2; thereafter a(n) = prime(mod(a(n-1),100))+1.
 * https://oeis.org/A112278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less, changed
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   get_prime, seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = 2; thereafter a(n) = prime(mod(a(n-1),100)) + 1. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap   = 0;

seq_t a(idx_t n)
{
    if (cache == NULL) {
        cache_cap   = 2;
        cache       = seq_malloc(cache_cap);
        cache[0]    = (seq_t)1;
        cache[1]    = (seq_t)2;
        cache_size  = 2;
    }
    if (n < cache_size)
        return cache[n];

    /* Ensure the cache is large enough */
    if (n + 1 > cache_cap) {
        cache_cap   = n + 1;
        cache       = seq_realloc(cache, cache_cap);
    }

    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        uint64_t idx_prime = (uint64_t)(prev % (seq_t)100);
        if (idx_prime == 0) {
            cache[i] = (seq_t)1;       /* Defend against an unlikely 0 index. */
        } else {
            seq_t p = get_prime(idx_prime);
            cache[i] = p + (seq_t)1;
        }
    }
    cache_size = n + 1;
    return cache[n];
}
