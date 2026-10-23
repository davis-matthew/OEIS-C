/*
 * A123982  Numbers k such that 12*k+1, 12*k+5 and 12*k+11 are primes.
 * https://oeis.org/A123982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer k such that 12*k+1, 12*k+5, and 12*k+11
 *            are all prime. The function searches for k sequentially, caching
 *            previously found terms
 * Range:     u8 n <= 25, u16 n <= 870, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t); */

static seq_t *cache = NULL;     /* cache of found terms */
static size_t cache_cap = 0;    /* current cache capacity */
static idx_t  cache_cnt = 0;    /* number of terms stored */
static idx_t  next_k    = 1;    /* next k to test */

/* enlarge cache to hold at least `need` elements */
static void ensure_cap(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 64;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* sequence entry */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not defined for offset 1; fallback */

    while (cache_cnt < n) {
        idx_t k = next_k++;          /* candidate k */

        uint64_t pk1 = 12ULL * (uint64_t)k + 1;
        uint64_t pk2 = 12ULL * (uint64_t)k + 5;
        uint64_t pk3 = 12ULL * (uint64_t)k + 11;

        if (is_prime((idx_t)pk1) &&
            is_prime((idx_t)pk2) &&
            is_prime((idx_t)pk3)) {
            ensure_cap(cache_cnt + 1);
            cache[cache_cnt++] = (seq_t)k;   /* store term */
        }
    }
    return cache[n - 1];
}
