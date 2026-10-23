/*
 * A035723  Coordination sequence for 28-dimensional cubic lattice.
 * https://oeis.org/A035723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   n*a(n) = 56*a(n-1) + (n-2)*a(n-2) (n > 1) * Initial values: a(0)
 *            = 1, a(1) = 56
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 25, u128 n <= 140,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;           /* array of computed values    */
static size_t cache_capacity = 0;     /* number of allocated slots   */
static idx_t last_index = (idx_t)-1;  /* highest index with a value  */

/* -------------------------------------------------------------- */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cache_capacity) return;

    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = seq_realloc(cache, cache_capacity, new_cap);
    if (tmp == NULL) {
        /* fallback: allocate new buffer and copy existing values */
        tmp = seq_alloc(new_cap);
        for (size_t i = 0; i < cache_capacity; ++i) {
            tmp[i] = cache[i];
        }
    }
    cache = tmp;
    cache_capacity = new_cap;
}

/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Base cases handled immediately. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)56;

    /* First call: allocate cache for base values. */
    if (cache == NULL) {
        cache_capacity = 2;
        cache = seq_alloc(cache_capacity);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)56;
        last_index  = 1;
    }

    /* Grow cache if necessary. */
    ensure_capacity(n);

    /* Compute missing values up to n. */
    if (n > last_index) {
        for (idx_t i = last_index + 1; i <= n; ++i) {
            if (sizeof(seq_t) <= 8) {
                /* Use 128‑bit intermediate for small integer types. */
                unsigned __int128 t1 = (unsigned __int128)56
                                     * (unsigned __int128)cache[i - 1];
                unsigned __int128 t2 = (unsigned __int128)(i - 2)
                                     * (unsigned __int128)cache[i - 2];
                unsigned __int128 sum = t1 + t2;
                cache[i] = (seq_t)(sum / (unsigned __int128)i);
            } else {
                /* Arbitrary‑precision multiplication and division. */
                seq_t t1 = (seq_t)56 * cache[i - 1];
                seq_t t2 = (seq_t)(i - 2) * cache[i - 2];
                cache[i] = (t1 + t2) / (seq_t)i;
            }
        }
        last_index = n;
    }

    return cache[n];
}
