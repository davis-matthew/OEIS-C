/*
 * A041758  Numerators of continued fraction convergents to sqrt(399).
 * https://oeis.org/A041758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 40*a(n-2) - a(n-4) for n ≥ 4, with a(0)=19, a(1)=20,
 *            a(2)=779, a(3)=799
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoized recurrence.  The environment supplies seq_t and idx_t. */
static seq_t *cache = NULL;          /* dynamically allocated cache */
static idx_t   computed = 0;         /* number of elements currently cached */

/* Base values for indices 0..3 */
static const seq_t base[4] = { 19, 20, 779, 799 };

/* Ensure the cache has space for at least (n+1) elements */
static void ensure_capacity(idx_t n)
{
    idx_t new_len = n + 1;
    if (cache == NULL)
    {
        cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!cache) abort();
    }
    else if (new_len > computed)
    {
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }
    /* If this is the first allocation, initialise the first four
       entries from the base array (they may have been needed). */
    if (computed == 0 && new_len >= 4)
    {
        for (idx_t i = 0; i < 4; ++i) cache[i] = base[i];
    }
}

/* Compute a(n) with memoisation */
seq_t a(idx_t n)
{
    /* Quick return if already cached */
    if (n < computed) return cache[n];

    /* Allocate space and pre-fill base if necessary */
    ensure_capacity(n);

    /* Compute missing terms incrementally */
    for (idx_t i = computed; i <= n; ++i)
    {
        if (i < 4)
            cache[i] = base[i];   /* safety; base already set if needed */
        else
            cache[i] = (seq_t)40 * cache[i-2] - cache[i-4];
    }

    computed = n + 1;
    return cache[n];
}
