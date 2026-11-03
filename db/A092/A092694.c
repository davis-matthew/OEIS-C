/*
 * A092694  Product of iterated phi(n).
 * https://oeis.org/A092694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = phi(n) * a(phi(n)), a(1) = 1
 * Range:     u8 n <= 16, u16 n <= 46, u32 n <= 256, u64 n <= 2328,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9771 (2026-11-03)
 * Helpers:   mem, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/totient.h"
#include "../utils/mem.h"

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

seq_t a(idx_t n) {
    /* Static cache for memoisation; indices match the OEIS offset. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    const size_t INITIAL_CAP = 100000;

    /* Initialise cache once, with zeroes and a(1)=1. */
    if (!cache) {
        cache_size = INITIAL_CAP;
        cache = seq_alloc_new_array(cache_size);
        mem_set(cache, 0, cache_size * sizeof(seq_t));
        cache[1] = 1;
    }

    /* Resize cache if necessary (support n larger than INITIAL_CAP). */
    if ((size_t)n >= cache_size) {
        size_t new_cap = n + 1;
        seq_t *new_cache = seq_alloc_new_array(new_cap);
        mem_copy(new_cache, cache, cache_size * sizeof(seq_t));
        mem_set(new_cache + cache_size, 0, (new_cap - cache_size) * sizeof(seq_t));
        free_seq_array(cache);
        cache = new_cache;
        cache_size = new_cap;
    }

    /* Return memoised value if available. */
    if (cache[n] != 0) return cache[n];

    /* Recursive computation: a(n) = phi(n) * a(phi(n)). */
    seq_t phi_val = totient(n);
    idx_t phi_n = (idx_t)phi_val;               /* phi(n) fits in idx_t for n>1 */
    seq_t result = phi_val * a(phi_n);          /* compute product in seq_t */

    cache[n] = result;                          /* memoise */
    return result;
}
