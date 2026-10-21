/*
 * A238759  The number of P-positions in the game of Nim with up to five
 *          piles, allowing for piles of zero, such that the total number of
 *          objects in all piles is 2n.
 * https://oeis.org/A238759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(2n+1) = 10 * a(n) a(2n+2) = a(n+1) + 5 * a(n)
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache       = NULL;
static size_t  cache_cap  = 0;   /* allocated size of cache      */
static size_t  computed   = 0;   /* number of cache entries already valid */

/* Ensure the cache has space for at least `needed` entries.
   The allocation is done using realloc, but the result must be
   cast explicitly to `seq_t *` to be C++ compliant. */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 4;
    if (new_cap < needed)
        new_cap = needed;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                           /* allocation failure */

    cache = new_cache;
    cache_cap = new_cap;
}

/* Main sequence function.  It accepts any unsigned 64‑bit index n
   (idx_t) and returns the corresponding value in seq_t.  The
   function is self‑contained, memoises results in a static cache,
   and correctly handles both C and C++ builds. */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;

    ensure_cache(idx + 1);

    /* Return cached result if already computed. */
    if (idx < computed)
        return cache[idx];

    /* Compute values from `computed` up to `idx`. */
    for (size_t i = computed; i <= idx; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)10;
        } else if (i & 1) {                 /* i is odd: 2k+1 */
            size_t k = (i - 1) / 2;
            cache[i] = cache[k] * (seq_t)10;
        } else {                            /* i is even: 2k+2 */
            size_t k = (i - 2) / 2;
            cache[i] = cache[k + 1] + (seq_t)5 * cache[k];
        }
    }

    computed = idx + 1;
    return cache[idx];
}
