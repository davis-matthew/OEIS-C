/*
 * A092532  G.f.: 1/((1-x)*(1-x^4)*(1-x^8)).
 * https://oeis.org/A092532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 119, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: dynamic programming on the recurrence
   a(n) = a(n-1) + a(n-4) - a(n-5) + a(n-8) - a(n-9) - a(n-12) + a(n-13)
   for n > 12 with initial terms
   a(0)=1, a(1)=1, a(2)=1, a(3)=1,
   a(4)=2, a(5)=2, a(6)=2, a(7)=2,
   a(8)=4, a(9)=4, a(10)=4, a(11)=4, a(12)=6. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for computed values */
    static seq_t *cache = NULL;     /* array of pre‑computed a(k) */
    static size_t capacity = 0;     /* allocated size of cache */
    static size_t len = 0;          /* number of actually stored terms (len == capacity on first fill) */

    /* Initial seed terms (0 … 12) */
    static const seq_t seed[13] = {
        1, 1, 1, 1,            /* 0..3 */
        2, 2, 2, 2,            /* 4..7 */
        4, 4, 4, 4, 6          /* 8..12 */
    };

    /* Lazily allocate and fill the cache with seed data */
    if (len == 0) {
        size_t init_cap = 64;               /* arbitrary start size */
        if (init_cap < 13) init_cap = 13;
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL) abort();         /* allocation failure */
        for (size_t i = 0; i < 13; ++i) cache[i] = seed[i];
        len = 13;
        capacity = init_cap;
    }

    /* Grow cache if needed */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = capacity ? capacity * 2 : 64;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();           /* allocation failure */
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute values up to n if not already computed */
    if ((size_t)n + 1 > len) {
        for (size_t i = len; i <= (size_t)n; ++i) {
            if (i < 13) {
                /* values 0..12 already initialized */
                continue;
            }
            /* recurrence: add the positive terms first to avoid intermediate underflow */
            seq_t val = cache[i-1] + cache[i-4] + cache[i-8] + cache[i-13];
            val -= cache[i-5];
            val -= cache[i-9];
            val -= cache[i-12];
            cache[i] = val;
        }
        len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
