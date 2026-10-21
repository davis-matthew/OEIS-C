/*
 * A082975  Denominators of continued fraction convergents to (sqrt(37)-4)/3.
 * https://oeis.org/A082975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 12*a(n-3) + a(n-6) for n ≥ 6, with initial values a(0)=1,
 *            a(1)=1, a(2)=3, a(3)=10, a(4)=13, a(5)=36 (OEIS A082975 –
 *            denominators of continued fraction convergents to
 *            (sqrt(37)-4)/3)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 26, u64 n <= 53, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache of computed terms */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* pre-initialisation of the first six terms */
static void init_first_terms(void) {
    seq_t init[6] = { (seq_t)1, (seq_t)1, (seq_t)3,
                     (seq_t)10, (seq_t)13, (seq_t)36 };
    if (!cache) {
        cache = (seq_t*)malloc(6 * sizeof(seq_t));
        if (!cache) { /* oops, allocation failed – return 0 in this unlikely case */
            return;
        }
    }
    for (idx_t i = 0; i < 6; ++i) {
        cache[i] = init[i];
    }
    cache_size = 6;
}

/* dynamic array expansion */
static void ensure_cache(idx_t n) {             /* n: index we need at least */
    if (cache_size > n) return;                /* already big enough */
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failed – silently fall back to 0 for all terms ≥ new_size */
        return;
    }
    cache = new_cache;
    /* initialise the first six terms if they weren't set yet */
    if (cache_size < 6) {
        init_first_terms();
    }
    /* compute missing terms */
    idx_t start = cache_size;
    if (start < 6) start = 6;                 /* skip already initialised terms */
    for (idx_t i = start; i <= n; ++i) {
        cache[i] = cache[i - 3] * (seq_t)12 + cache[i - 6];
    }
    cache_size = new_size;
}

seq_t a(idx_t n) {
    if (n < 6) {                              /* fast path for the known prefix */
        static const seq_t init[6] = { (seq_t)1, (seq_t)1, (seq_t)3,
                                        (seq_t)10, (seq_t)13, (seq_t)36 };
        return init[n];
    }
    ensure_cache(n);
    if (n < cache_size) return cache[n];
    /* if allocation failed, this function may reach here; return 0 */
    return (seq_t)0;
}
