/*
 * A169619  Hankel transform of quartic (or 4-fold) factorial numbers A007696.
 * https://oeis.org/A169619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A169619: Hankel transform of quartic factorial numbers
 * Formula: a(n) = ∏_{k=0..n} ((4*k+1)*(4*k+4))^(n-k)   (n ≥ 0)
 * This implementation uses the recurrence
 *   a(0) = 1
 *   a(n) = a(n-1) * ∏_{k=0}^{n-1} ((4k+1)*(4k+4))
 * By keeping incremental products in static state, any order of calls
 * is supported and the cache grows only up to the maximum requested n.
 */

#include <stdlib.h>
#include "../utils/seq_mul.h"   /* provides seq_mul(seq_t, seq_t) */

seq_t a(idx_t n)
{
    /* Cache of pre‑computed a(k) values */
    static seq_t *cache   = NULL;
    static idx_t  cache_max = 0;
    static size_t  cache_cap = 0;

    /* Incremental multiplicative state
       cur_a = a(cache_max)
       prod  = ∏_{k=0}^{cache_max-1} ((4k+1)*(4k+4))   ( = M(cache_max-1) )
    */
    static seq_t cur_a   = (seq_t)1;   /* a(0) = 1 */
    static seq_t prod    = (seq_t)1;   /* product for a(0) *)

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) return cur_a;         /* out of memory – unlikely */
        cache[0] = cur_a;                        /* a(0) = 1 */
        cache_max = 0;
    }

    /* Return cached value if already computed */
    if (n <= cache_max) {
        return cache[n];
    }

    /* Ensure cache has enough space for new entries */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)(n + 1) > new_cap) new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (new_cache == NULL) return cur_a;     /* realloc failed */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute a(k) for k = cache_max+1 .. n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* T = (4*(i-1)+1)*(4*(i-1)+4)  (fits in 32 bits for i≤10000) */
        idx_t k = i - 1;
        unsigned long long T = (4ULL * k + 1ULL) * (4ULL * k + 4ULL);

        /* Update running product prod = ∏_{j=0}^{i-1} ((4j+1)*(4j+4)) */
        prod = seq_mul(prod, (seq_t)T);

        /* Update a(i) = a(i-1) * prod */
        cur_a = seq_mul(cur_a, prod);

        /* Store result in cache */
        cache[i] = cur_a;
    }

    cache_max = n;
    return cur_a;
}
