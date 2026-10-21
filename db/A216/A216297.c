/*
 * A216297  Values of k such that 10k + 7 is the only prime between 10k and
 *          10k + 9.
 * https://oeis.org/A216297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 7084, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: search for k such that 10k+7 is the only prime between
   10k and 10k+9.  The values are kept in a static cache so that
   calls with arbitrary arguments still return correctly. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* cache of computed a(n) values          */
static size_t cache_len = 0;  /* number of entries currently cached     */
static size_t cache_cap = 0;  /* allocated capacity of the cache        */
static uint64_t last_k = 0;   /* last k examined by the search          */

/* Simple deterministic primality test for 64‑bit integers. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

/* Grow the internal cache to at least new_cap elements. */
static bool grow_cache(size_t new_cap)
{
    void *tmp = realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return false;
    cache = (seq_t *)tmp;
    cache_cap = new_cap;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n < 1           */

    /* Ensure we have at least n entries in the cache. */
    while (cache_len < (size_t)n) {
        ++last_k;
        uint64_t m7 = last_k * 10ULL + 7ULL;
        uint64_t m1 = last_k * 10ULL + 1ULL;
        uint64_t m3 = last_k * 10ULL + 3ULL;
        uint64_t m9 = last_k * 10ULL + 9ULL;

        /* 10k+7 must be prime. */
        if (!is_prime_uint64(m7)) continue;

        /* The other odd numbers 10k+1, 10k+3, 10k+9 must be composite. */
        if (is_prime_uint64(m1)) continue;
        if (is_prime_uint64(m3)) continue;
        if (is_prime_uint64(m9)) continue;

        /* Store the valid k. */
        if (cache_len == cache_cap) {
            if (!grow_cache(cache_cap ? cache_cap * 2 : 4))
                return 0;   /* allocation failure - return 0 as fallback  */
        }
        cache[cache_len++] = (seq_t)last_k;
    }

    return cache[n - 1];
}
