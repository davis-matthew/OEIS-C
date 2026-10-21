/*
 * A098300  Member r=15 of the family of Chebyshev sequences S_r(n) defined in
 *          A092184.
 * https://oeis.org/A098300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(n) = 13*a(n‑1) - a(n‑2) + 2 for n ≥ 2
 *            (Recurrence definition from OEIS A098300)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;   /* dynamic array of computed terms   */
static size_t cache_len = 0;  /* number of terms stored (indices 0..len-1) */

/* Ensure the cache can hold terms up to index n.
   Previous values are preserved; new entries are uninitialized. */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;

    /* allocate or grow the cache to size n+1 */
    size_t new_len = (size_t)n + 1;
    seq_t *new_cache = NULL;
    if (cache == NULL) {
        /* Initial allocation with size 2 (for a(0), a(1))   */
        new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
    } else {
        new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    }
    if (new_cache == NULL) {
        /* Allocation failed – program cannot continue safely.
           In typical build environments this should not happen for n up to ~10000. */
        abort();
    }
    cache = new_cache;
    if (cache_len == 0) {
        /* set initial values if this is the very first allocation   */
        cache[0] = (seq_t)0;
        if (new_len > 1)
            cache[1] = (seq_t)1;
    } else {
        /* if cache was already allocated, ensure first two values are set
           (this can happen if a(0) or a(1) is requested after some grow) */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }
    /* compute missing terms from old_len to new_len-1 */
    for (size_t i = cache_len; i < new_len; ++i) {
        if (i == 0)
            cache[i] = (seq_t)0;
        else if (i == 1)
            cache[i] = (seq_t)1;
        else
            cache[i] = (seq_t)13 * cache[i-1] - cache[i-2] + (seq_t)2;
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
