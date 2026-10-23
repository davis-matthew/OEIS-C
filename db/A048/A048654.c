/*
 * A048654  a(n) = 2*a(n-1) + a(n-2); a(0)=1, a(1)=4.
 * https://oeis.org/A048654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2), a(0)=1, a(1)=4
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 24, u64 n <= 49, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n‑th term of OEIS sequence A048654. */
seq_t a(idx_t n)
{
    /* Persistent cache for computed terms. */
    static seq_t *cache = NULL;
    static idx_t cap = 0;      /* capacity of the cache array */
    static idx_t computed = 0; /* number of terms that have been computed */

    /* First call – initialise the cache with the two seed values. */
    if (cap == 0) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;          /* out of memory */
        cache[0] = (seq_t)1;           /* a(0) */
        cache[1] = (seq_t)4;           /* a(1) */
        computed = 2;
    }

    /* If the requested index is already computed, return it directly. */
    if (n < computed)
        return cache[n];

    /* Ensure the cache has enough storage for index n. */
    if (n >= cap) {
        idx_t new_cap = cap;
        if (new_cap == 0) new_cap = 2;
        while (new_cap <= n) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* out of memory */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute all missing terms up to n. */
    for (idx_t i = computed; i <= n; ++i) {
        /* a(i) = 2*a(i-1) + a(i-2) */
        cache[i] = ((seq_t)2 * cache[i - 1]) + cache[i - 2];
    }
    computed = n + 1;

    return cache[n];
}
