/*
 * A142216  Primes congruent to 19 mod 41.
 * https://oeis.org/A142216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 19 modulo 41 (OEIS A142216)
 * Range:     u8 n <= 2, u16 n <= 143, u32 n <= 159, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) */

/* External linkage:  seq_t a(idx_t n) */
seq_t a(idx_t n)
{
    /* Static cache of primes found so far */
    static seq_t *cache = NULL;        /* array of sequence values */
    static size_t cache_len = 0;       /* number of primes stored */
    static size_t cache_cap = 0;       /* allocated capacity of cache */
    static idx_t next_k = 0;           /* next k for candidate 41*k+19 */

    /* Grow cache until it holds the nth term */
    while (cache_len < n) {
        uint64_t k      = next_k++;                /* current k (uint64_t suffices) */
        uint64_t val    = 41u * k + 19u;            /* candidate integer */
        seq_t   cand    = (seq_t)val;              /* cast to seq_t for testing */

        /* Test primality */
        if (is_prime_big(cand)) {
            /* Allocate more space if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;           /* allocation failure – abort */
                cache = tmp;
                cache_cap = new_cap;
            }
            /* Store the found prime */
            cache[cache_len++] = cand;
        }
    }

    /* Return the requested term (sequence indices are 1‑based) */
    return cache[n - 1];
}
