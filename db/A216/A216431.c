/*
 * A216431  a(0)=0; thereafter a(n+1) = a(n) + 1 + number of 0's in binary
 *          representation of a(n), counted with A023416.
 * https://oeis.org/A216431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0; a(n+1) = a(n) + 1 + (# of 0‑bits in binary expansion
 *            of a(n))
 * Range:     u8 n <= 67, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   zero_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Supports re‑ordered calls; memoises computed terms. */

#include <stdlib.h>   /* malloc, realloc */
#include "../utils/zero_bits.h"

static seq_t *cache = NULL;          /* memoised sequence values     */
static size_t cache_cap = 0;          /* number of stored terms        */

seq_t a(idx_t n)
{
    /* Ensure the cache contains entries up to index n. */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        if (cache == NULL) {
            /* Out of memory: return 0 as a safe fallback. */
            return (seq_t)0;
        }

        /* Initialise missing terms. */
        for (idx_t i = (idx_t)cache_cap; i <= n; ++i) {
            if (i == 0) {                     /* a(0) = 0 */
                cache[0] = (seq_t)0;
                continue;
            }
            seq_t prev   = cache[i - 1];
            seq_t z      = zero_bits(prev);
            cache[i] = prev + (seq_t)1 + z;
        }
        cache_cap = new_cap;
    }
    return cache[(size_t)n];
}
