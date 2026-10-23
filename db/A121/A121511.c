/*
 * A121511  a(n) = a(n-1) + a(n-4) - a(n-5).
 * https://oeis.org/A121511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-4) - a(n-5); offset 1 a(1) = 1, a(2) = 5,
 *            a(3) = 13, a(4) = 25, a(5) = 5
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t last = 0;     /* highest index computed so far */
    static idx_t cap   = 0;    /* current allocated capacity (indices 0..cap) */

    if (n == 0) return 0;      /* caller should use only n >= 1 */

    /* Initialization on the first call */
    if (cache == NULL) {
        cap = 6; /* allocate indices 0..5 (index 0 unused) */
        cache = (seq_t*)malloc((cap + 1) * sizeof(seq_t));
        if (!cache) abort();   /* unlikely but safe */

        /* Base values */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)13;
        cache[4] = (seq_t)25;
        cache[5] = (seq_t)5;
        last = 5;
    }

    /* If the requested index is already known, return it */
    if (n <= last) return cache[n];

    /* Grow the cache if needed */
    if (n > cap) {
        idx_t newcap = cap;
        while (newcap < n) newcap <<= 1;   /* double until large enough */
        seq_t *tmp = (seq_t*)realloc(cache, (newcap + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = newcap;
    }

    /* Compute missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 4] - cache[i - 5];
    }
    last = n;
    return cache[n];
}
