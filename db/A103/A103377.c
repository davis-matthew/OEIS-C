/*
 * A103377  a(1)=a(2)=...=a(10)=1, a(n)=a(n-9)+a(n-10).
 * https://oeis.org/A103377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-9) + a(n-10), with a(1)=…=a(10)=1
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Cache of previously computed terms.  The cache expands only
     * when larger indices are requested.  It remains valid for
     * all subsequent calls.  */
    static seq_t *cache = NULL;
    static idx_t  cache_cap = 0;  /* allocated size */
    static idx_t  cache_size = 0; /* highest index that has been computed (1‑based) */

    if (n == 0)          /* out of range but safe for unsigned n */
        return 0;

    /* First call: initialise the cache with the first ten ones. */
    if (cache == NULL) {
        cache_cap  = 10;
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 10; ++i) cache[i] = 1;
        cache_size = 10;        /* indices 1‑–10 are known */
    }

    /* If the requested term is already known, return it. */
    if (n <= cache_size)
        return cache[n - 1];

    /* Compute all missing terms up to n, extending the cache if needed. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i > cache_cap) {                     /* extend the cache */
            idx_t new_cap = cache_cap * 2;
            if (new_cap < i) new_cap = i;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }
        idx_t idx = i - 1;                        /* zero‑based index */
        /* recurrence: a(i) = a(i-9) + a(i-10)  for i > 10 */
        cache[idx] = cache[idx - 9] + cache[idx - 10];
    }
    cache_size = n;
    return cache[n - 1];
}
