/*
 * A142094  Primes congruent to 27 mod 35.
 * https://oeis.org/A142094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 236, u32 n <= 248, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth prime p with p ≡ 27 (mod 35) */
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static idx_t tested = 0;          /* number of candidates inspected */

    /* first‑time allocation */
    if (cache == NULL) {
        cache_cap = 1024;                     /* initial capacity */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) return (seq_t)0;   /* allocation failure */
    }

    /* generate primes until we have at least n of them */
    while (cache_len < n) {
        /* candidate of the form 27 + 35*k, where k = tested */
        seq_t candidate = (seq_t)(27 + (seq_t)35 * tested);

        if (is_prime_big(candidate)) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap << 1;
                seq_t *tmp = (seq_t *)malloc(sizeof(seq_t) * new_cap);
                if (tmp == NULL) return (seq_t)0; /* allocation failure */
                memcpy(tmp, cache, sizeof(seq_t) * cache_len);
                free(cache);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = candidate;
        }
        tested++; /* advance to the next candidate */
    }

    return cache[n - 1];
}
