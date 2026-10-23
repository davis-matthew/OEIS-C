/*
 * A130800  Numbers k such that both 2k+1 and 3k+1 are primes.
 * https://oeis.org/A130800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 517, u32 n <= 518, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n‑th integer k (1‑based) such that both 2k+1
   and 3k+1 are prime numbers. */
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static state to allow out‑of‑order calls */
    static seq_t *cache      = NULL;   /* array of found k's          */
    static size_t cache_cap  = 0;      /* capacity of cache           */
    static idx_t  cache_cnt  = 0;      /* number of terms stored      */
    static seq_t  next_k     = 1;      /* next candidate k to test    */

    /* Allocate initial cache if needed */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;  /* allocation failure */
    }

    /* Produce terms until we have at least n of them */
    while (cache_cnt < n) {
        /* Check primality of 2k+1 and 3k+1 */
        seq_t two_k_plus_one = (seq_t)2 * next_k + (seq_t)1;
        seq_t three_k_plus_one = (seq_t)3 * next_k + (seq_t)1;

        if (is_prime_big(two_k_plus_one) && is_prime_big(three_k_plus_one)) {
            /* Grow cache if necessary */
            if (cache_cnt >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) return 0;  /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            /* Store the found k */
            cache[cache_cnt++] = next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
