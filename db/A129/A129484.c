/*
 * A129484  Primes of the form 17k + 1.
 * https://oeis.org/A129484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the nth prime of the form 17k+1 (offset 1)
 * Range:     u8 n <= 3, u16 n <= 415, u32 n <= 44, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form 17k+1 are searched incrementally and cached. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t); */

seq_t a(idx_t n)
{
    /* undefined behaviour allowed for n = 0; return 0 for safety */
    if (n == 0) return (seq_t)0;

    /* static cache of found primes */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t next_k = 1;   /* the k used for the next candidate */

    /* initialise cache on first entry */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure */
    }

    /* extend cache until it contains the n‑th prime */
    while (cache_len < n) {
        /* candidate of the form 17k + 1 */
        uint64_t cand64 = 17ULL * next_k + 1ULL;   /* fits in uint64_t for needed ranges */
        seq_t cand = (seq_t)cand64;

        if (is_prime_big(cand)) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) return (seq_t)0;   /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
        ++next_k;
    }

    return cache[n - 1];
}
