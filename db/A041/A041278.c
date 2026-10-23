/*
 * A041278  Numerators of continued fraction convergents to sqrt(152).
 * https://oeis.org/A041278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=12, a(1)=37, a(2)=900, a(3)=2737, a(n)=74*a(n-2)-a(n-4) for
 *            n>3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache of previously computed terms (size = cache_len). */
static seq_t *cache = NULL;
static size_t cache_len = 0;       /* allocated length (number of slots)   */
static size_t computed_upto = 0;   /* highest index that has a valid value */

/* Ensure that cache has space for indices 0..need and that the base
   values 0..3 are initialized.  Extra slots are filled with 0. */
static void ensure_cache(size_t need)
{
    if (need < cache_len)
        return;                       /* already large enough */

    size_t new_len = need + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    cache = new_cache;

    /* Initialize new slots with 0. */
    for (size_t i = cache_len; i < new_len; ++i)
        cache[i] = 0;

    /* If this is the first allocation, store the initial four terms. */
    if (cache_len == 0) {
        cache[0] = (seq_t)12;
        cache[1] = (seq_t)37;
        cache[2] = (seq_t)900;
        cache[3] = (seq_t)2737;
        computed_upto = 3;
    }

    cache_len = new_len;
}

/* Return the n-th term of A041278.  The function is thread‑safe
   only if the caller serialises access to it. */
seq_t a(idx_t n)
{
    /* Base indices need to be available for the recurrence. */
    ensure_cache(3);

    size_t idx = (size_t)n;
    ensure_cache(idx);

    /* If we already computed this index, return it. */
    if (idx <= computed_upto)
        return cache[idx];

    /* Compute missing terms up to idx. */
    for (size_t i = computed_upto + 1; i <= idx; ++i) {
        /* The recurrence is defined only for i>=4, but the loop
           starts at computed_upto+1, which is at least 4 after the
           initial base case. */
        cache[i] = (seq_t)(74 * cache[i - 2] - cache[i - 4]);
    }

    computed_upto = idx;
    return cache[idx];
}
