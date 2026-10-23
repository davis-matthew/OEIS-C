/*
 * A136169  a(n) = 2*a(n-1) - [(n+1)/3] for n>0 with a(0) = 1.
 * https://oeis.org/A136169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - floor((n+1)/3), a(0) = 1
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_n = 0;

    /* initialise cache for a(0) */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failure */
        cache[0] = (seq_t)1;
        cap    = 1;
        max_n  = 0;
    }

    /* grow cache if needed */
    if (n > max_n) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap > cap) {
            seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp) return (seq_t)0;          /* allocation failure */
            cache = tmp;
            cap   = new_cap;
        }

        /* compute missing values using the recurrence */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t two = cache[i - 1] + cache[i - 1];          /* 2*a(i-1) */
            seq_t sub = (seq_t)((i + 1) / 3);                 /* floor((i+1)/3) */
            cache[i]  = two - sub;
        }
        max_n = n;
    }

    return cache[n];
}
