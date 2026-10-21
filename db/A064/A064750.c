/*
 * A064750  a(n) = n*12^n + 1.
 * https://oeis.org/A064750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*12^n + 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence used for efficient computation:
   a(0) = 1, a(1) = 13, a(2) = 289
   a(n) = 25*a(n-1) - 168*a(n-2) + 144*a(n-3)  for n >= 3
   (all arithmetic is performed modulo the width of seq_t) */

#include <stdlib.h>

/* Cached sequence values */
static seq_t *a_cache = NULL;         /* array of computed terms */
static idx_t  cache_capacity = 0;     /* allocated size of a_cache        */
static idx_t  computed_len   = 0;     /* number of terms actually computed */

/* Ensure capacity of the cache and compute terms up to n if necessary */
static void ensure_and_compute(idx_t n) {
    /* Grow cache if needed */
    if (cache_capacity <= (size_t)n) {
        idx_t new_cap = cache_capacity > 0 ? cache_capacity : 3;
        while (new_cap <= n) new_cap *= 2;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        cache_capacity = new_cap;
    }

    /* First time: initialize base terms */
    if (computed_len == 0) {
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)13;
        a_cache[2] = (seq_t)289;
        computed_len = 3;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = computed_len; i <= n; ++i) {
        if (i < 3) continue;               /* base terms already set */
        /* recurrence: a(i) = 25*a(i-1) - 168*a(i-2) + 144*a(i-3) */
        seq_t val = (25 * a_cache[i-1]) - (168 * a_cache[i-2]) + (144 * a_cache[i-3]);
        a_cache[i] = val;
    }
    computed_len = n + 1;
}

seq_t a(idx_t n) {
    ensure_and_compute(n);
    return a_cache[n];
}
