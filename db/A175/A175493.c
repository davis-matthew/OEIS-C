/*
 * A175493  a(n) = Product_{k=1..n} k^d(k), where d(k) = number of divisors of
 *          k.
 * https://oeis.org/A175493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} k^{d(k)}, where d(k) = number of divisors of k
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 117 (2026-11-03)
 * Helpers:   ipow, mem, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/num_divisors.h"
#include "../utils/mem.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }
#include <stddef.h>                     /* size_t */

/* Compute a(n) for n ≥ 1.  This implementation memoises values
   in a static dynamic array that grows as required.  It works
   for any seq_t (integer, fixed‑width or arbitrary‑precision). */
seq_t
a(idx_t n)
{
    /* The sequence is defined for n ≥ 1 only. */
    if (n < 1)
        return (seq_t)0;

    /* Static cache: cache[i‑1] holds a(i). */
    static seq_t *cache = NULL;   /* pointer to the array */
    static idx_t  cache_cap = 0;  /* current storage capacity */
    static idx_t  max_cached = 0; /* largest n already computed */

    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache_cap   = 1;
        cache       = seq_alloc_new_array(cache_cap);
        cache[0]    = (seq_t)1;     /* a(1) = 1^1 = 1 */
        max_cached  = 1;
    }

    /* Grow the cache if we need to store more terms. */
    if (n > cache_cap) {
        /* Double the capacity or set to n, whichever is larger. */
        idx_t new_cap = n > cache_cap * 2 ? n : cache_cap * 2;
        seq_t *new_cache = seq_alloc_new_array(new_cap);
        /* Copy the already computed terms into the new array. */
        for (idx_t i = 0; i < max_cached; ++i)
            new_cache[i] = cache[i];
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute terms up to n if not already done. */
    if (n > max_cached) {
        for (idx_t k = max_cached + 1; k <= n; ++k) {
            uint64_t d = num_divisors(k);           /* d(k) */
            seq_t pow_k = ipow((seq_t)k, (seq_t)d);   /* k^{d(k)} */
            cache[k-1] = seq_mul(cache[k-2], pow_k);     /* a(k) = a(k-1)*k^{d(k)} */
        }
        max_cached = n;
    }

    return cache[n-1];
}
