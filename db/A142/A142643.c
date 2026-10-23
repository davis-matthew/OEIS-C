/*
 * A142643  Primes congruent to 5 mod 56.
 * https://oeis.org/A142643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes congruent to 5 mod 56 (OEIS A142643)
 * Range:     u8 n <= 4, u16 n <= 267, u32 n <= 175, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_big.h"

/* static cache of computed terms */
static seq_t *cache = NULL;      /* array of seq_t */
static size_t cache_len = 0;     /* number of primes stored */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static idx_t next_k = 0;         /* next k for candidate = 5 + 56*k */

/* Return the n‑th prime that is 5 (mod 56), where a(1)=5 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* undefined for n<1, guard useless */

    if ((size_t)n <= cache_len)
        return cache[n - 1];         /* already computed */

    /* generate primes until we have at least n terms */
    while (1) {
        uint64_t candidate = 5ULL + (uint64_t)next_k * 56ULL;
        next_k++;

        seq_t cand = (seq_t)candidate;
        if (is_prime_big(cand)) {    /* true for any seq_t (small or big) */

            /* grow cache if needed */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 32;
                seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
                if (!tmp) abort();
                if (cache) {
                    memcpy(tmp, cache, cache_len * sizeof(seq_t));
                    free(cache);
                }
                cache = tmp;
                cache_cap = new_cap;
            }

            cache[cache_len++] = cand;

            if ((size_t)n <= cache_len)
                return cache[n - 1];
        }
    }
}
