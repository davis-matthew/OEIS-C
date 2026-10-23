/*
 * A170473  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   bn, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (from the generating function):
   a(n) = b(n) + 30 * Σ_{k=1}^{44} a(n-k) – 465 * a(n-45)
   where
     b(0) = 1, 1 ≤ b(n) ≤ 44 = 2, b(45) = 1, and b(n) = 0 otherwise.   */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/bn.h"

static seq_t *cache = NULL;        /* array of computed values          */
static size_t  cache_cap = 0;      /* current capacity of cache         */
static idx_t   cache_used = 0;     /* highest index that has been computed */

seq_t a(idx_t n)
{
    /* Grow the cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = n + 1;
        /* Allocate a new array and copy the old values */
        seq_t *new_cache = seq_alloc(new_cap);
        if (cache) {
            for (idx_t i = 0; i < cache_used; ++i)
                new_cache[i] = cache[i];
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing entries */
    for (idx_t i = cache_used; i <= n; ++i) {
        seq_t sum = 0;
        for (int k = 1; k <= 44; ++k) {
            if (i >= (idx_t)k)
                sum += cache[i - k];
        }
        seq_t val = bn(i) + (seq_t)30 * sum;
        if (i >= 45)
            val -= (seq_t)465 * cache[i - 45];
        cache[i] = val;
    }
    cache_used = n + 1;

    return cache[n];
}
