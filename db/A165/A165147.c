/*
 * A165147  a(n) = (3*7^n-3^n)/2.
 * https://oeis.org/A165147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A165147 : a(n) = (3*7^n - 3^n)/2
   Recurrence: a(0)=1, a(1)=9, a(n)=10*a(n-1)-21*a(n-2) for n>1 */

#include <stdlib.h>  /* malloc, free, abort */

seq_t a(idx_t n) {
    /* Static cache for already computed values. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* capacity of cache array */
    static idx_t  sz    = 0;   /* number of values currently stored (0..sz-1) */

    /* Handle base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;

    /* Ensure capacity for index n. */
    if ((size_t)n + 1 > cap) {
        size_t newcap = (cap == 0) ? 1 : cap;
        while (newcap <= (size_t)n) newcap <<= 1;  /* double until big enough */
        seq_t *tmp = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (!tmp) abort();                     /* allocation failed */
        if (cache) {
            for (size_t i = 0; i < cap; ++i) tmp[i] = cache[i];
            free(cache);
        }
        cache = tmp;
        cap   = newcap;
    }

    /* Initialise base values if not already done. */
    if (sz == 0) {
        cache[0] = (seq_t)1;
        sz = 1;
    }
    if (sz == 1) {
        cache[1] = (seq_t)9;
        sz = 2;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = sz; i <= n; ++i) {
        seq_t t1 = cache[i - 1] * 10;
        seq_t t2 = cache[i - 2] * 21;
        cache[i] = t1 - t2;               /* recurrence: 10*a(i-1)-21*a(i-2) */
    }
    sz = n + 1;  /* now we have values 0..n */

    return cache[n];
}
