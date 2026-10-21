/*
 * A226076  Lexicographically least sequence of squares that are sum-free.
 * https://oeis.org/A226076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(1..9) as listed above; for n > 9, a(n) = 4 * a(n-3).
 *            This matches the observed pattern (conjecture) for the
 *            lexicographically least sequence of squares that is sum‑free
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache for memoization. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Initial nine terms of the sequence. */
static const seq_t init[9] = {
    (seq_t)1,   /* a(1) */
    (seq_t)4,   /* a(2) */
    (seq_t)9,   /* a(3) */
    (seq_t)16,  /* a(4) */
    (seq_t)36,  /* a(5) */
    (seq_t)64,  /* a(6) */
    (seq_t)144, /* a(7) */
    (seq_t)256, /* a(8) */
    (seq_t)289  /* a(9) */
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* behavior below offset is ignored */

    /* Grow cache if necessary */
    if (cache_len < n) {
        idx_t new_len = n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* allocation failure → 0 */
        cache = new_cache;

        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i < 9) {
                cache[i] = init[i];
            } else {
                cache[i] = (seq_t)4 * cache[i - 3];
            }
        }
        cache_len = new_len;
    }
    return cache[n - 1];
}
