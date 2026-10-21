/*
 * A087592  Primes whose successive differences are increasing power of 2:
 *          a(1) = 2, a(n+1) = a(n) + 2^k; a(n+1) prime, k minimal and greater
 *          than the index for the previous term.
 * https://oeis.org/A087592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   is_prime_big, pow2, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A087592: Primes whose successive differences are increasing powers of two.
 * a(1) = 2, a(2) = 3.
 * For n > 2: let p(n-1) be a(n-1) and k1 the exponent used to reach p(n-1).
 *           a(n) = p(n-1) + 2^k where k is the smallest integer > k1 such that the sum is prime.
 * The function caches previously computed terms and remembers the last exponent
 * so that any n ≥ 1 can be answered in order‑independent fashion.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/pow2.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Static cache of computed terms and associated state */
    static seq_t *cache     = NULL;
    static idx_t cache_len  = 0;   /* number of terms stored */
    static idx_t cache_cap  = 0;   /* allocated capacity of cache */
    static idx_t prev_exp   = 0;   /* exponent used to obtain last stored term */

    /* Return cached value if already computed */
    if (cache_len >= n)
        return cache[n - 1];

    /* Initialise cache if this is the first call */
    if (!cache) {
        cache_cap = 16;
        cache = seq_alloc(cache_cap);

        /* a(1) = 2, a(2) = 3 */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)3;
        cache_len = 2;
        prev_exp  = 0;   /* exponent used to reach a(2) is 0 (2^0 = 1) */
    }

    /* Generate terms up to the requested index */
    while (cache_len < n) {
        /* Grow cache if necessary */
        if (cache_len == cache_cap) {
            cache_cap = cache_cap * 2;
            cache = seq_realloc(cache, cache_len, cache_cap);
        }

        /* Find the smallest exponent k > prev_exp such that
           cache[cache_len-1] + 2^k is prime. */
        seq_t prev = cache[cache_len - 1];
        idx_t k = prev_exp + 1;
        seq_t candidate;
        for (;;) {
            candidate = prev + pow2(k);
            if (is_prime_big(candidate)) {
                cache[cache_len] = candidate;
                prev_exp = k;
                break;
            }
            ++k;
        }
        ++cache_len;
    }

    return cache[n - 1];
}
