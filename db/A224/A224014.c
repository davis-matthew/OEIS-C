/*
 * A224014  Number of 4 X n 0..2 arrays with rows nondecreasing and
 *          antidiagonals unimodal.
 * https://oeis.org/A224014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 26, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) + 126*a(n-5)
        - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9)
   Initial values (for 1 ≤ n ≤ 11):
   a(1)=81, a(2)=1296, a(3)=7378, a(4)=28541, a(5)=90051, a(6)=245055,
   a(7)=595822, a(8)=1325316, a(9)=2742301, a(10)=5343468, a(11)=9896484 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* trivial outside the offset: behaviour not specified */
    if (n == 0) return 0;

    /* static memoisation cache */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;          /* number of cached terms */

    /* the first eleven terms */
    static const seq_t init[11] = {
        (seq_t)81,
        (seq_t)1296,
        (seq_t)7378,
        (seq_t)28541,
        (seq_t)90051,
        (seq_t)245055,
        (seq_t)595822,
        (seq_t)1325316,
        (seq_t)2742301,
        (seq_t)5343468,
        (seq_t)9896484
    };

    /* initialise the cache on first call */
    if (cache == NULL) {
        cache_size = 11;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed */
        for (idx_t i = 0; i < 11; ++i)
            cache[i] = init[i];
    }

    /* return cached value if available */
    if (n <= cache_size)
        return cache[n-1];

    /* grow the cache to hold n terms */
    size_t new_size = (size_t)n;
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return 0;              /* allocation failed */
    cache = tmp;

    /* compute missing terms using the 9‑term recurrence */
    for (idx_t k = cache_size + 1; k <= n; ++k) {
        if (k <= 11) {               /* safety guard, should not occur */
            cache[k-1] = init[k-1];
            continue;
        }
        /* compute using only non‑negative arithmetic */
        seq_t pos = cache[k-2] * (seq_t)9   + cache[k-4] * (seq_t)84
                  + cache[k-6] * (seq_t)126 + cache[k-8] * (seq_t)36
                  + cache[k-10] * (seq_t)1;
        seq_t neg = cache[k-3] * (seq_t)36 + cache[k-5] * (seq_t)126
                  + cache[k-7] * (seq_t)84  + cache[k-9] * (seq_t)9;
        cache[k-1] = pos - neg;         /* guaranteed non‑negative */
    }

    cache_size = new_size;
    return cache[n-1];
}
