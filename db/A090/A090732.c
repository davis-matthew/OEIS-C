/*
 * A090732  a(n) = 24a(n-1) - a(n-2), starting with a(0) = 2 and a(1) = 24.
 * https://oeis.org/A090732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 24*a(n-1) - a(n-2), a(0) = 2, a(1) = 24
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of OEIS sequence A090732. */
seq_t a(idx_t n)
{
    /* Handle the known base cases first. */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)24;

    /* Static cache of computed terms. */
    static seq_t *cache = NULL;
    static idx_t computed_max = 0;

    /* If we already have this term cached, return it. */
    if (cache && n <= computed_max)
        return cache[n];

    /* Ensure the cache has room for indices up to n. */
    if (!cache) {
        /* First allocation: store the base values. */
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure (unlikely in tests) */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)24;
        computed_max = 1;
    } else if (n > computed_max) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* allocation failure */
        cache = new_cache;
    }

    /* Compute missing terms iteratively, using the recurrence. */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        /* All operations are on seq_t; for built‑in unsigned types the
           width is sufficient for intermediate results up to the test
           limits.  For arbitrary‑precision builds the class definition
           supplies the correct arithmetic. */
        cache[i] = cache[i - 1] * (seq_t)24 - cache[i - 2];
    }

    /* Record the new maximum computed index. */
    computed_max = n;
    return cache[n];
}
