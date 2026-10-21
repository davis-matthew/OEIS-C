/*
 * A074826  Binomial transform of reflected pentanacci numbers A074062: a(n) =
 *          Sum_{k=0..n}(-1)^k*binomial(n, k)*A074062(k).
 * https://oeis.org/A074826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 15, i32 n <= 27, i64 n <= 57, i128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3)
   - 15*a(n-4) + 4*a(n-5)
   Initial values: a(0)=5, a(1)=6, a(2)=6, a(3)=6, a(4)=6 */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;     /* cached values */
static idx_t capacity = 0;      /* number of cache slots (>= last_computed+1) */
static idx_t last_computed = 0; /* highest index that has been computed */
static bool initialized = false;/* flag that the first 5 terms are stored */

static void ensure_cache(idx_t n) {
    if (n < capacity) return;          /* already enough */

    idx_t new_cap = n + 1;              /* need indices 0..n inclusive */
    seq_t *new_buf = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
    if (!new_buf) { /* allocation failed: leave cache unchanged; subsequent
                        accesses will crash in a controlled way */ }
    /* copy existing terms */
    if (cache) {
        for (idx_t i = 0; i < capacity; ++i) new_buf[i] = cache[i];
        free(cache);
    }
    cache = new_buf;
    capacity = new_cap;
}

seq_t a(idx_t n) {
    /* initialise the first five terms once */
    if (!initialized) {
        ensure_cache(4); /* indices 0..4 */
        cache[0] = (seq_t)5; /* a(0) */
        cache[1] = (seq_t)6; /* a(1) */
        cache[2] = (seq_t)6; /* a(2) */
        cache[3] = (seq_t)6; /* a(3) */
        cache[4] = (seq_t)6; /* a(4) */
        last_computed = 4;
        initialized = true;
    }

    /* if the requested index is already cached, return it */
    if (n <= last_computed) return cache[n];

    /* make sure the cache can hold up to index n */
    if (n >= capacity) ensure_cache(n);

    /* compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* recurrence is valid for i >= 5 */
        seq_t t1 = (seq_t)6    * cache[i - 1];
        seq_t t2 = (seq_t)15   * cache[i - 2];
        seq_t t3 = (seq_t)20   * cache[i - 3];
        seq_t t4 = (seq_t)15   * cache[i - 4];
        seq_t t5 = (seq_t)4    * cache[i - 5];
        cache[i] = t1 - t2 + t3 - t4 + t5;
    }
    last_computed = n;
    return cache[n];
}
