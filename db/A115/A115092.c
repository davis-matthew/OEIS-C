/*
 * A115092  The number of m such that prime(n) divides m!+1.
 * https://oeis.org/A115092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of m such that prime(n) divides m!+1 (for 1≤m<p)
 * Range:     u8 n <= 54, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

static seq_t *cache = NULL;   /* cached a(n) values, 0‑based */
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Expand the cache array to at least `new_cap` elements.  */
static void ensure_cache_cap(size_t new_cap)
{
    if (cache_cap >= new_cap)
        return;
    size_t new_capacity = cache_cap ? cache_cap * 2 : 4;
    while (new_capacity < new_cap)
        new_capacity *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, new_capacity * sizeof(seq_t));
    if (!tmp)
        return; /* allocation failed – caller will handle with zeroed value */
    cache = tmp;
    cache_cap = new_capacity;
}

/* Compute a(n) for all indices up to n and store them in cache. */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;

    ensure_cache_cap((size_t)n);

    for (size_t i = cache_len; i < (size_t)n; ++i) {
        idx_t kth = (idx_t)(i + 1);                 /* prime index (1‑based) */
        uint32_t pk = get_prime(kth);               /* k‑th prime, fits 32‑bit */
        uint64_t p  = (uint64_t)pk;                 /* convert to 64‑bit */

        seq_t count = 0;
        seq_t fact  = 1;

        for (idx_t m = 1; m < (idx_t)p; ++m) {
            uint64_t prod = (uint64_t)fact * (uint64_t)m;
            fact = (seq_t)(prod % p);
            if (fact == (seq_t)(p - 1))
                ++count;
        }
        cache[i] = count;
    }
    cache_len = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n=0, return 0 for safety */

    ensure_cache(n);
    return cache[(size_t)n - 1];
}
