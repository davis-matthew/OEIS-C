/*
 * A140848  Primes of the form 210k + 41.
 * https://oeis.org/A140848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 139, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form 210*k + 41.  Each term is the n‑th
   prime p such that p ≡ 41 (mod 210).  (Offset 1) */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* declares is_prime(idx_t) */

seq_t a(idx_t n)
{
    /* static cache of generated terms */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_cnt  = 0;
    static idx_t   k_next    = 0;   /* next k to test */

    if (n == 0) return 0;          /* undefined for n < 1 */

    /* generate terms until we have at least n of them */
    while (cache_cnt < (size_t)n) {
        /* candidate number: 210*k + 41 */
        idx_t candidate = 210ULL * k_next + 41ULL;

        /* test primality using the utility routine */
        if (is_prime(candidate)) {
            /* grow cache if necessary */
            if (cache_cnt >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();   /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            /* store the prime (may overflow seq_t for large terms,
               but this is harmless for values that fit in seq_t) */
            cache[cache_cnt++] = (seq_t)candidate;
        }
        k_next++;
    }

    return cache[n - 1];
}
