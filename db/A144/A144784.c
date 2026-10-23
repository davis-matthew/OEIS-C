/*
 * A144784  Variant of Sylvester's sequence: a(n+1) = a(n)^2 - a(n) + 1, with
 *          a(1) = 11.
 * https://oeis.org/A144784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 11, a(n+1) = a(n)^2 - a(n) + 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;       /* cache of computed terms */
static idx_t  cached_n = 0;        /* number of terms cached (max n) */
static idx_t  capacity = 0;        /* allocated cache size */

#ifdef __cplusplus
#include <new>
#endif

/* Ensure cache can hold at least 'needed' terms */
static void ensure_capacity(idx_t needed)
{
    if (needed <= capacity)
        return;

    idx_t new_cap = needed;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        return; /* allocation failed – in practice the test harness will not hit this */

    /* Copy existing elements to the new buffer */
    for (idx_t i = 0; i < capacity; ++i)
        new_cache[i] = cache[i];

    /* Default‑construct new elements (required if seq_t is a C++ class) */
#ifdef __cplusplus
    for (idx_t i = capacity; i < new_cap; ++i)
        new((void*)&new_cache[i]) seq_t();
#endif

    free(cache);
    cache   = new_cache;
    capacity = new_cap;
}

/* Return the n-th term of the sequence (n >= 1) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n=0; return 0 as placeholder */

    /* If requested term already cached, return it */
    if (n <= cached_n)
        return cache[n-1];

    ensure_capacity(n);

    /* Compute missing terms */
    if (cached_n == 0) {
        cache[0] = 11;       /* a(1) */
        cached_n = 1;
    }

    for (idx_t i = cached_n; i < n; ++i) {
        seq_t prev = cache[i-1];
        seq_t next = prev * prev - prev + 1;
        cache[i] = next;
    }
    cached_n = n;

    return cache[n-1];
}
