/*
 * A223882  Number of n X 2 0..2 arrays with row sums unimodal and column sums
 *          inverted unimodal.
 * https://oeis.org/A223882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 14, u64 n <= 33, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A223882: Number of n×2 0..2 arrays with row sums unimodal and column sums
   inverted unimodal.
   Empirical recurrence for n ≥ 1:
        a(n) = 17*a(n-1) - 126*a(n-2) + 534*a(n-3) - 1425*a(n-4)
             + 2481*a(n-5) - 2816*a(n-6) + 2008*a(n-7)
             - 816*a(n-8) + 144*a(n-9)
   Initial values (a(1)..a(9)) are 9,81,564,3298,17048,80412,353485,1469903,
   5845411.  The function memoises results so that calls may arrive in any
   order.  The implementation is fully compliant with the build‑time
   definitions of seq_t and idx_t. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Memoisation cache: cache[0] unused, cache[i] holds a(i) for i ≥ 1. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* largest n already cached */

/* Initial values a(1) … a(9) */
static const seq_t init[9] = {
    (seq_t)9,(seq_t)81,(seq_t)564,(seq_t)3298,(seq_t)17048,
    (seq_t)80412,(seq_t)353485,(seq_t)1469903,(seq_t)5845411
};

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                 /* undefined for n<1 */

    /* Grow cache if necessary */
    if (n > cache_size) {
        size_t new_cap = (size_t)(n + 1);        /* include index 0 */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;        /* allocation failed */
        cache = new_cache;

        /* Compute missing terms */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i <= 9) {
                cache[i] = init[i-1];
            } else {
                seq_t pos = (seq_t)17 * cache[i-1];
                pos += (seq_t)534 * cache[i-3];
                pos += (seq_t)2481 * cache[i-5];
                pos += (seq_t)2008 * cache[i-7];
                pos += (seq_t)144 * cache[i-9];

                seq_t neg = (seq_t)126 * cache[i-2];
                neg += (seq_t)1425 * cache[i-4];
                neg += (seq_t)2816 * cache[i-6];
                neg += (seq_t)816 * cache[i-8];

                cache[i] = pos - neg;
            }
        }
        cache_size = n;
    }

    return cache[n];
}
