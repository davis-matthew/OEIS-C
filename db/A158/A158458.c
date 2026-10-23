/*
 * A158458  Numbers k such that k + bigomega(k) is prime.
 * https://oeis.org/A158458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that k + bigomega(k) is prime (OEIS A158458)
 * Range:     u8 n <= 52, u16 n <= 6007, u32 n <= 6245, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/bigomega.h"
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;          /* stored sequence values */
static size_t cache_len = 0;         /* number of terms stored */
static size_t cache_cap = 0;         /* allocated capacity of cache */
static idx_t  processed_k = 1;       /* next integer to test (start with 2) */

/* Ensure that cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        ++processed_k;                             /* candidate integer k */
        seq_t omega = bigomega(processed_k);        /* Ω(k) */
        seq_t cand  = (seq_t)processed_k + omega;   /* k + Ω(k) */

        if (is_prime_big(cand)) {                  /* check primality */
            if (cache_len == cache_cap) {           /* grow cache if needed */
                cache_cap = cache_cap ? cache_cap * 2 : 64;
                cache = (seq_t*)realloc(cache,
                                         cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)processed_k;/* store term */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, but safe guard */
    ensure_cache(n);
    return cache[n - 1];
}
