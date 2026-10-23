/*
 * A043853  Numbers k such that number of runs in the base 4 representation of
 *          k is congruent to 3 mod 8.
 * https://oeis.org/A043853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer k (k ≥ 1) whose base‑4 representation *
 *            has a number of runs congruent to 3 (mod 8). A run is a maximal
 *            block * of equal digits in the base‑4 expansion. The
 *            implementation * iteratively searches for such integers while
 *            memoising the results. * The function is O(final_outpu...
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static unsigned int count_runs(seq_t x) {
    unsigned int runs = 0;
    unsigned int prev = 4;          /* sentinel (outside 0..3) */
    while (x > 0) {
        unsigned int d = (unsigned int)(x % (seq_t)4);
        if (d != prev) {
            runs++;
            prev = d;
        }
        x /= (seq_t)4;
    }
    return runs;
}

seq_t a(idx_t n) {
    /* static cache for memoised results */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t last_checked = 0;   /* k last examined */

    /* initialise cache lazily */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }

    /* grow cache if needed */
    if (n > cache_cap) {
        while (cache_cap < n) cache_cap <<= 1;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* extend cache until we have at least n elements */
    while (cache_len < n) {
        last_checked++;             /* examine next integer */
        if (count_runs((seq_t)last_checked) % 8 == 3) {
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
