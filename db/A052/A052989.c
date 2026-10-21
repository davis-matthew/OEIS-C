/*
 * A052989  Expansion of ( 1-x ) / ( 1-x-x^2-x^4+x^5 ).
 * https://oeis.org/A052989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Recurrence * a(0) = 1, a(1) = 0, a(2) = 1, a(3) = 1, a(4) = 3 *
 *            a(n+5) = a(n+1) + a(n+3) + a(n+4) - a(n) (n ≥ 0) * * This
 *            computes the OEIS sequence A052989: Expansion of
 *            (1‑x)/(1‑x‑x²‑x⁴+x⁵)
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation.  The array holds the sequence values
       up to the largest index that has been computed so far. */
    static seq_t *cache = NULL;      /* pointer to cached values          */
    static size_t cache_cap = 0;     /* allocated capacity (in elements) */
    static size_t computed_up_to = 0;/* largest index already filled      */

    /* First call: allocate a small buffer and initialise the first
       five known terms. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure – return 0 as a sentinel */

        cache[0] = (seq_t)1; /* a(0) */
        cache[1] = (seq_t)0; /* a(1) */
        cache[2] = (seq_t)1; /* a(2) */
        cache[3] = (seq_t)1; /* a(3) */
        cache[4] = (seq_t)3; /* a(4) */
        computed_up_to = 4;
    }

    /* Ensure the internal buffer is large enough for the requested index. */
    size_t idx = (size_t)n;
    if (idx >= cache_cap) {
        size_t new_cap = (cache_cap > 0) ? (cache_cap << 1) : 16;
        while (new_cap <= idx) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* reallocation failure – return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If the value is already cached, just return it. */
    if (idx <= computed_up_to) return cache[idx];

    /* Compute the missing terms up to idx using the recurrence. */
    for (size_t i = computed_up_to + 1; i <= idx; ++i) {
        /* For i < 5 the value is already in cache; the loop body only
           runs for i ≥ 5, where the recurrence applies. */
        cache[i] = cache[i-4] + cache[i-2] + cache[i-1] - cache[i-5];
    }

    computed_up_to = idx;
    return cache[idx];
}
