/*
 * A031473  Numbers whose base-5 representation has 3 more 0's than 4's.
 * https://oeis.org/A031473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer whose base‑5 representation contains
 *            exactly 3 more 0’s than 4’s
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Helper to test whether a number satisfies the condition. */
static bool satisfies(seq_t x) {
    seq_t t = x;
    int diff = 0;              /* zeros minus fours */
    while (t > 0) {
        seq_t d = t % 5;
        if (d == 0) {
            diff++;
        } else if (d == 4) {
            diff--;
        }
        t /= 5;
    }
    return diff == 3;
}

/* Publicly visible sequential generator. */
seq_t a(idx_t n) {
    if (n == 0) return 0;      /* off‑by‑one safety, not used */

    /* Static cache of previously generated terms. */
    static seq_t *cache = NULL;      /* array of terms */
    static idx_t cache_len = 0;      /* number of known terms */
    static idx_t cache_cap = 0;      /* allocated size of cache */

    /* Ensure the cache contains at least n terms. */
    if (cache_len < n) {
        /* Grow capacity if necessary. */
        if (cache_cap == 0) cache_cap = 64;
        while (cache_cap < n) cache_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* allocation error */
        cache = tmp;

        /* Continue searching from the last known value. */
        seq_t val = 0;
        if (cache_len > 0) val = cache[cache_len - 1];

        /* Generate missing terms. */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            do {
                ++val;
            } while (!satisfies(val));
            cache[i - 1] = val;
        }

        cache_len = n;
    }
    return cache[n - 1];
}
