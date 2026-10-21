/*
 * A208318  Number of n X 4 0..2 arrays with new values 0..2 introduced in row
 *          major order and no element equal to more than two of its immediate
 *          leftward or upward or left-upward diagonal neighbors.
 * https://oeis.org/A208318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 65*a(n-1) + 600*a(n-2) + 320*a(n-3) - 4224*a(n-4) +
 *            1728*a(n-5) for n>6 Initial terms: a(1)=14, a(2)=982,
 *            a(3)=71956, a(4)=5269968, a(5)=385979024, a(6)=28269518640
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* cache[n-1] holds a(n) */
static idx_t cache_cap = 0;          /* allocated capacity of cache */

/* Grow cache to at least 'needed' elements */
static void grow_cache(idx_t needed) {
    if (needed <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : needed;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!tmp) {
        /* Allocation failed; in contest environment this should not happen */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialize cache with initial terms */
static void init_cache(void) {
    grow_cache(6);
    if (cache[0] == 0) { /* not yet initialized */
        cache[0] = (seq_t)14;                  /* a(1) */
        cache[1] = (seq_t)982;                 /* a(2) */
        cache[2] = (seq_t)71956;               /* a(3) */
        cache[3] = (seq_t)5269968;             /* a(4) */
        cache[4] = (seq_t)385979024;           /* a(5) */
        cache[5] = (seq_t)28269518640;         /* a(6) */
    }
}

/* Compute the OEIS sequence A208318 */
seq_t a(idx_t n) {
    if (n == 0) { /* undefined, return 0 */
        return (seq_t)0;
    }
    init_cache();
    if (n <= 6) {
        return cache[n-1];
    }
    grow_cache(n);
    for (idx_t i = 6; i < n; ++i) {          /* i = index (0-based) of a(n) */
        if (cache[i] == 0) {                 /* compute only if not already */
            seq_t sum = (seq_t)65 * cache[i-1]
                      + (seq_t)600 * cache[i-2]
                      + (seq_t)320 * cache[i-3]
                      + (seq_t)1728 * cache[i-5];
            cache[i] = sum - (seq_t)4224 * cache[i-4];
        }
    }
    return cache[n-1];
}
