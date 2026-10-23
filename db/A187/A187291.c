/*
 * A187291  Number of 6-step one or two space at a time rook's tours on an n X
 *          n board summed over all starting positions.
 * https://oeis.org/A187291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) for n>12, with initial
 *            values: a(1)=0, a(2)=0, a(3)=1440, a(4)=15424, a(5)=57288,
 *            a(6)=134408, a(7)=248208, a(8)=397152, a(9)=580328,
 *            a(10)=797160, a(11)=1047532, a(12)=1331408
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static memoisation table */
    static seq_t *cache = NULL;     /* array indexed by n (1‑based) */
    static idx_t cache_size = 0;    /* array length */
    static idx_t computed_max = 0;  /* highest n computed so far */

    /* Initialise table with the 12 known values if not yet done */
    if (cache == NULL) {
        /* Allocate at least 13 entries (index 0 unused) */
        cache_size = 13;
        cache = (seq_t *)calloc((size_t)cache_size, sizeof(seq_t));
        if (cache == NULL) return 0; /* out of memory, but we cannot return NULL */

        cache[1] = 0;
        cache[2] = 0;
        cache[3] = (seq_t)1440;
        cache[4] = (seq_t)15424;
        cache[5] = (seq_t)57288;
        cache[6] = (seq_t)134408;
        cache[7] = (seq_t)248208;
        cache[8] = (seq_t)397152;
        cache[9] = (seq_t)580328;
        cache[10] = (seq_t)797160;
        cache[11] = (seq_t)1047532;
        cache[12] = (seq_t)1331408;
        computed_max = 12;
    }

    /* If requested n is already computed, return it */
    if (n <= computed_max) {
        return cache[n];
    }

    /* Grow the array if necessary */
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0; /* allocation failure */
        cache = tmp;
        /* Initialise any new entries to zero (realloc does not zero new memory) */
        for (size_t i = cache_size; i < new_size; ++i)
            cache[i] = 0;
        cache_size = new_size;
    }

    /* Compute values up to n using the recurrence */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        cache[i] = 3 * cache[i - 1] - 3 * cache[i - 2] + cache[i - 3];
    }
    computed_max = n;

    return cache[n];
}
