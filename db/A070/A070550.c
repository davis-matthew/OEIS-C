/*
 * A070550  a(n) = a(n-1) + a(n-3) + a(n-4), starting with a(0..3) = 1, 2, 2,
 *          3.
 * https://oeis.org/A070550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4), a(0)=1, a(1)=2, a(2)=2, a(3)=3
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cached = 0;          /* number of elements already stored */

    /* ensure at least 4 initial values are available */
    if (n < 4) {
        if (cached >= n+1) return cache[n];
        if (!cache) cache = (seq_t*)malloc(4 * sizeof(seq_t));
        else cache = (seq_t*)realloc(cache, 4 * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        cached = 4;
        return cache[n];
    }

    /* enlarge cache if necessary */
    if (cached <= n) {
        /* if first time, allocate at least 4 entries */
        if (!cache) cache = (seq_t*)malloc(4 * sizeof(seq_t));
        if (cached < 4) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)2;
            cache[2] = (seq_t)2;
            cache[3] = (seq_t)3;
            cached = 4;
        }
        /* allocate enough space for up to n */
        cache = (seq_t*)realloc(cache, (size_t)(n+1) * sizeof(seq_t));

        /* compute missing terms */
        for (idx_t i = cached; i <= n; ++i) {
            /* recurrence: a[i] = a[i-1] + a[i-3] + a[i-4] */
            cache[i] = cache[i-1] + cache[i-3] + cache[i-4];
        }
        cached = n + 1;
    }

    return cache[n];
}
