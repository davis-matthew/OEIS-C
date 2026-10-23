/*
 * A118924  Primes for which the weight as defined in A117078 is 53 and the
 *          gap as defined in A001223 is 52.
 * https://oeis.org/A118924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A118924: primes p such that p ≡ 52 (mod 53) and the next prime after p is p + 52. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */

/* Cached terms and generation state */
static seq_t *cache = NULL;
static size_t  cache_cap = 0;
static size_t  cache_cnt = 0;
static idx_t   next_k   = 0;          /* next k to try in 53*k + 52 */
static bool    initialized = false;

/* Ensure capacity for at least 'need' elements in the cache */
static void ensure_cache(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < need) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – abort */
            exit(1);
        }
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return the n-th term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for 0, guard */

    /* Lazy initialisation */
    if (!initialized) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_cnt = 0;
        next_k   = 0;
        initialized = true;
    }

    /* Generate terms until we have at least n of them */
    size_t need = (size_t)n;
    while (cache_cnt < need) {
        /* Candidate prime in the progression 53*k + 52 */
        seq_t p = ((seq_t)53 * (seq_t)next_k) + (seq_t)52;

        /* Check primality of p */
        if (is_prime_big(p)) {
            /* Find the first prime after p (search only up to p+52) */
            seq_t q            = p + (seq_t)2;
            seq_t p_plus_52    = p + (seq_t)52;
            bool found_target  = false;

            while (q <= p_plus_52) {
                if (is_prime_big(q)) {
                    if (q == p_plus_52) found_target = true;
                    break;   /* first prime after p found */
                }
                q += (seq_t)2;
            }

            if (found_target) {
                ensure_cache(cache_cnt + 1);
                cache[cache_cnt++] = p;
            }
        }

        next_k++;  /* move to the next number in the progression */
    }

    return cache[n - 1];   /* 1‑based indexing */
}
