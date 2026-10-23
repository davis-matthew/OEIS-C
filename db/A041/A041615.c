/*
 * A041615  Denominators of continued fraction convergents to sqrt(326).
 * https://oeis.org/A041615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 650*a(n-2) - a(n-4) for n > 3 Initial values: a(0)=1,
 *            a(1)=18, a(2)=649, a(3)=11700
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* External definitions provided by the build system: */
/* typedef ... seq_t;   // integer type (uint8_t, uint16_t, ...,
   unsigned __int128 or a bigint class)               */
/* typedef ... idx_t;   // unsigned 64‑bit (uint64_t) */

static seq_t *cache = NULL;      /* array of computed terms          */
static size_t cache_cap = 0;     /* allocated capacity (elements)   */
static size_t cache_len = 0;     /* number of terms stored          */

/* Ensure the cache has at least 'needed' slots.  If necessary,
   reallocates and copies existing values.  The cache array
   holds seq_t elements.                                                */
static void ensure_cache_size(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = (cache_cap > 0) ? cache_cap : 4;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        abort(); /* Allocation failure – terminate. */

    /* Copy existing elements if any. */
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    /* Free old cache array if it exists. */
    if (cache)
        free(cache);

    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialise the cache with the first four known terms. */
static void init_cache(void)
{
    if (cache == NULL) {
        ensure_cache_size(4);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)18;
        cache[2] = (seq_t)649;
        cache[3] = (seq_t)11700;
        cache_len = 4;
    }
}

/* Return the n-th term of the sequence.  The function is
   safe for any non‑negative n and can be called in arbitrary order. */
seq_t a(idx_t n)
{
    /* Cast idx_t to size_t for array indexing – safe as n ≤ 2^64-1. */
    size_t idx = (size_t)n;

    /* Special‑case trivial small n to avoid unnecessary allocation. */
    if (idx <= 3) {
        if (!cache) init_cache(); /* make sure cache is created, though not used */
        return cache[idx];
    }

    /* Ensure cache is initialised and large enough. */
    init_cache();
    if (idx >= cache_len) {
        ensure_cache_size(idx + 1);
        for (size_t i = cache_len; i <= idx; ++i) {
            /* Recurrence: a[i] = 650*a[i-2] - a[i-4] */
            cache[i] = (650 * cache[i - 2]) - cache[i - 4];
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
