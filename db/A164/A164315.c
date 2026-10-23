/*
 * A164315  Number of binary strings of length n with no substrings equal to
 *          000 or 011.
 * https://oeis.org/A164315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-4) (for n ≥ 4)
 * Range:     u8 n <= 14, u16 n <= 34, u32 n <= 74, u64 n <= 152,
 *            u128 n <= 310, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*        a(0) = 1, a(1) = 2, a(2) = 4, a(3) = 6          */
/* Number of binary strings of length n with no substrings 000 or 011 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache of computed terms */
static idx_t cache_size = 0;  /* number of elements stored */

seq_t a(idx_t n) {
    size_t idx = (size_t)n;           /* convert to size_t for array ops */

    /* Extend cache if necessary */
    if (idx + 1 > cache_size) {
        size_t old_size = cache_size;
        size_t new_size = idx + 1;
        if (new_size < 4) new_size = 4;   /* at least cover base cases */

        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        /* Copy existing values */
        for (size_t i = 0; i < old_size; ++i)
            new_cache[i] = cache[i];

        /* Free old cache */
        free(cache);
        cache = new_cache;
        cache_size = new_size;

        /* Set base values for indices 0..3 (if within new_size) */
        if (new_size >= 1) cache[0] = (seq_t)1;
        if (new_size >= 2) cache[1] = (seq_t)2;
        if (new_size >= 3) cache[2] = (seq_t)4;
        if (new_size >= 4) cache[3] = (seq_t)6;

        /* Compute remaining terms using the recurrence */
        for (size_t i = 4; i < new_size; ++i) {
            seq_t val = cache[i-1] + cache[i-2];
            val -= cache[i-4];
            cache[i] = val;
        }
    }

    return cache[(size_t)n];
}
