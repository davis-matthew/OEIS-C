/*
 * A193071  Odd numbers N for which sigma(N^2) is not prime.
 * https://oeis.org/A193071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth odd integer N such that σ(N^2) is not a prime
 * Range:     u8 n <= 6, u16 n <= 149, u32 n <= 149, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, seq_alloc, seq_realloc, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate odd candidates, compute σ(N²) via sigma_of,
 * test primality with is_prime_big, and cache found terms. */

#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache for previously found terms */
    static seq_t *cache      = NULL;
    static idx_t cache_len   = 0;
    static idx_t cache_cap   = 0;
    static idx_t next_candidate = 1;  /* next odd number to test */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = seq_alloc(cache_cap);
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        idx_t cand = next_candidate;                 /* current odd N */
        idx_t sq   = cand * cand;                    /* N^2 (fits in idx_t) */
        seq_t sigma = sigma_of(sq);                  /* σ(N²) */

        /* include candidate if σ(N²) is composite */
        if (!is_prime_big(sigma)) {
            if (cache_len == cache_cap) {            /* grow cache if needed */
                idx_t new_cap = cache_cap * 2;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_len++] = (seq_t)cand;
        }

        next_candidate += 2;                         /* next odd number */
    }

    return cache[n - 1];
}
