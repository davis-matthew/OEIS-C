/*
 * A049809  a(n)=number of Farey fractions of order n that are <=1/5; cf.
 *          A049805.
 * https://oeis.org/A049809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A049809: a(n) = number of Farey fractions of order n that are <= 1/5. */
/* Definitions: a(1) = 1, a(n) = a(n-1) + #{ k : 1 <= k <= ⌊n/5⌋ and gcd(k,n)=1 } for n>1. */
#include <stdlib.h>
#include <stdint.h>

/* Helper: Euclidean GCD for idx_t values. */
static inline idx_t _gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    /* Handle zero index (outside official offset). */
    if (n == 0) return 0;

    /* Static storage for computed values. */
    static seq_t *cache = NULL;
    static idx_t capacity = 0;   /* allocated size (number of elements, cache[0] unused) */
    static idx_t computed = 0;   /* highest index computed so far (>=1 after first call) */

    /* Grow cache if needed. */
    if (capacity < n + 1) {
        idx_t new_cap = capacity ? capacity : 8;
        while (new_cap < n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed */
        cache = tmp;
        capacity = new_cap;
    }

    /* Initialise first term if not yet done. */
    if (computed == 0) {
        cache[1] = (seq_t)1;
        computed = 1;
    }

    /* Incrementally compute new values. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        idx_t limit = i / 5;          /* ⌊i/5⌋ */
        seq_t cnt = 0;
        for (idx_t k = 1; k <= limit; ++k) {
            if (_gcd_idx(i, k) == 1) {
                ++cnt;
            }
        }
        cache[i] = cache[i - 1] + cnt;
    }

    computed = n;
    return cache[n];
}
