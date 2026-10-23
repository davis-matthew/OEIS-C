/*
 * A135749  a(n) = Sum_{k=0..n} binomial(n,k)*(n-k)^k*k^k.
 * https://oeis.org/A135749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * (n-k)^k * k^k
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"   /* provides binomial(idx_t,idx_t) -> seq_t */
#include "../utils/big_pow.h"    /* provides big_pow(seq_t, seq_t) -> seq_t */

seq_t a(idx_t n)
{
    /* Static cache to keep already computed terms; works for any call order. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    /* Ensure the cache can hold index n, enlarging if needed. */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                     /* allocation failure */
        /* initialise new slots to zero */
        for (idx_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If already computed, return the cached value. */
    if (cache[n] != (seq_t)0)
        return cache[n];

    /* Compute the sum over k = 0 .. n. */
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t bin = binomial(n, k);

        /* Compute (n-k)^k and k^k.  big_pow handles any exponent (=k) that
           fits in seq_t, which is fine for the required small seq_t values
           and exact for the arbitrary‑precision build. */
        seq_t term1 = big_pow((seq_t)(n - k), (seq_t)k);
        seq_t term2 = big_pow((seq_t)k, (seq_t)k);

        /* term = bin * term1 * term2 */
        seq_t term = bin * term1 * term2;
        sum += term;
    }

    /* Memoise and return. */
    cache[n] = sum;
    return sum;
}
