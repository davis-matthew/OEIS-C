/*
 * A128231  Expansion of exp(x)/(1 - x^3/3!), where a(n) = 1 +
 *          binomial(n,3)*a(n-3).
 * https://oeis.org/A128231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A128231:  Expansion of exp(x)/(1 - x^3/3!)
 * Recurrence: a(0)=a(1)=a(2)=1,
 *             a(n) = 1 + binomial(n,3)*a(n-3)  for n ≥ 3
 */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;   /* array holding a(0)..a(max_computed) */
    static size_t capacity = 0;   /* current allocated size of cache */
    static idx_t  max_computed = (idx_t)-1;  /* last index that has been filled */

    /* First‑time initialisation – we always need space for at least 3 terms */
    if (max_computed == (idx_t)-1) {
        /* Determine the minimal capacity required for the initial three terms
         * plus any requested index (n).  The new capacity must be at least 3. */
        size_t needed = (n >= 2) ? (size_t)n + 1 : 3;
        if (capacity < needed) {
            seq_t *tmp = seq_realloc(cache, capacity, needed);
            if (!tmp) return (seq_t)0;   /* allocation failure – should not happen */
            cache    = tmp;
            capacity = needed;
        }
        /* Initialise the base cases */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        max_computed = 2;
    }

    /* Ensure the cache can hold up to index n */
    if (capacity <= n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = seq_realloc(cache, capacity, new_cap);
        if (!tmp) return (seq_t)0;
        cache    = tmp;
        capacity = new_cap;
    }

    /* If the requested term is already computed, return it */
    if (n <= max_computed)
        return cache[n];

    /* Compute the sequence values from max_computed+1 up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* binomial(i,3) = i*(i-1)*(i-2)/6 */
        seq_t coeff = (seq_t)i * (seq_t)(i - 1) * (seq_t)(i - 2) / (seq_t)6;
        cache[i]    = (seq_t)1 + coeff * cache[i - 3];
    }

    max_computed = n;
    return cache[n];
}
