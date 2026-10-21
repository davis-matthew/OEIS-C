/*
 * A079446  Integers k such that phi(k)*(k-phi(k))+1 is prime.
 * https://oeis.org/A079446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 342, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_ull, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A079446
   a(n) = the nth integer k such that φ(k)*(k-φ(k))+1 is prime.
   The function generates the terms on demand and caches them.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/phi.h"
#include "../utils/is_prime_ull.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;  /* number of stored terms */
    static idx_t  cache_cap = 0;  /* allocated capacity of cache   */
    static idx_t  next_k = 1;     /* next integer to test (prev. 1) */

    /* initialise on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    /* generate more terms until we have at least n of them */
    while (cache_len < n) {
        ++next_k;                            /* test next integer */

        seq_t phi_k = phi(next_k);           /* φ(next_k)          */
        seq_t prod   = phi_k * ((seq_t)next_k - phi_k); /* φ(k)*(k-φ(k)) */
        seq_t val    = prod + 1;             /* φ(k)*(k-φ(k))+1   */

        if (is_prime_ull((uint64_t)val)) {
            /* enlarge cache if necessary */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = next_k;       /* store found term */
        }
    }

    return cache[n - 1];
}
