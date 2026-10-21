/*
 * A063060  'Reverse and Add!' trajectory of 10553.
 * https://oeis.org/A063060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(0) = 10553, a(n) = a(n-1) + reverse_digits(a(n-1)) for n > 0
 *            (reverse_digits: decimal reversal of the integer)
 * Range:     u8 none, u16 n <= 1, u32 n <= 12, u64 n <= 35, u128 n <= 78,
 *            big any n
 * Helpers:   rev_digits_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/rev_digits_seq_t.h"

/* Cache of previously computed terms. */
static seq_t *cache  = NULL;   /* array of computed terms */
static idx_t cache_len  = 0;   /* number of terms stored */
static idx_t cache_cap  = 0;   /* current capacity of the array */

seq_t a(idx_t n) {
    /* Ensure the cache is initialized. */
    if (cache == NULL) {
        cache_cap = (idx_t)8;
        cache = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        if (cache == NULL) {
            /* Allocation failure: return 0 (should never happen in tests). */
            return (seq_t)0;
        }
        cache[0] = (seq_t)10553;   /* a(0) */
        cache_len = (idx_t)1;
    }

    /* If requested term is already computed, return it. */
    if (n < cache_len) {
        return cache[n];
    }

    /* Extend the cache if necessary. */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (n >= new_cap) {
            new_cap *= (idx_t)2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failure: return 0. */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == (idx_t)0) {
            /* Term 0 already initialized. */
            continue;
        }
        seq_t prev = cache[i - (idx_t)1];
        seq_t rev  = rev_digits_seq_t(prev);
        seq_t next = prev + rev;
        cache[i] = next;
    }
    cache_len = n + (idx_t)1;
    return cache[n];
}
