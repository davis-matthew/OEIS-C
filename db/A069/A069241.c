/*
 * A069241  Number of Hamiltonian paths in the graph on n vertices {1,...,n},
 *          with i adjacent to j iff |i-j| <= 2.
 * https://oeis.org/A069241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) - 2*a(n-4) + a(n-5) * with
 *            initial values a(0)=1, a(1)=1, a(2)=1, a(3)=3, a(4)=6
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 55, u64 n <= 113,
 *            u128 n <= 229, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows from the generating function
 *     (3+x+x^2)/(1-x-x^3) - (2-x)/(1-x)^2
 * whose denominator gives the above 5‑term linear recurrence. */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Helper to set base values for indices 0..4 */
static void init_cache(void) {
    if (cache_len == 0) {
        cache_len = 5;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return;           /* silently fail if memory exhausted */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)6;
    }
}

static void extend_cache(idx_t n) {
    size_t new_len = (size_t)(n + 1);
    if (new_len <= cache_len) return;
    seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) return;           /* silently fail if memory exhausted */
    cache = new_cache;
    /* compute values from old_len to new_len-1 */
    for (size_t i = cache_len; i < new_len; ++i) {
        /* val = 3*cache[i-1] + 2*cache[i-3] + cache[i-5] */
        seq_t val = cache[i - 1];
        val += cache[i - 1];          /* 2*cache[i-1] */
        val += cache[i - 1];          /* 3*cache[i-1] */

        seq_t tmp = cache[i - 3];
        tmp += tmp;                   /* 2*cache[i-3] */
        val += tmp;

        val += cache[i - 5];

        /* sub = 3*cache[i-2] + 2*cache[i-4] */
        seq_t sub = cache[i - 2];
        sub += cache[i - 2];
        sub += cache[i - 2];          /* 3*cache[i-2] */

        seq_t tmp2 = cache[i - 4];
        tmp2 += tmp2;                 /* 2*cache[i-4] */
        sub += tmp2;

        val -= sub;                   /* recurrence */
        cache[i] = val;
    }
    cache_len = new_len;
}

seq_t a(idx_t n) {
    /* handle initial indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)6;

    init_cache();
    extend_cache(n);
    return cache[(size_t)n];
}
