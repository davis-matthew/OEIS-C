/*
 * A222867  Number of n X 1 0..5 arrays with no element equal to another at a
 *          city block distance of exactly two, and new values 0..5 introduced
 *          in row major order.
 * https://oeis.org/A222867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 41*a(n-2) + 61*a(n-3) - 30*a(n-4) for n>6 *
 *            Initial values: a(1)=1, a(2)=2, a(3)=3, a(4)=7, a(5)=20, a(6)=67
 *            * * Sequence definition: Number of n×1 0..5 arrays with no
 *            element equal to * another at a city block distance of exactly
 *            two, new values 0..5 * introduced in row major...
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 17, u64 n <= 31, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (n == 0) /* Undefined for offset 1, but return 0 for safety. */
        return (seq_t)0;

    /* Extend cache if necessary. */
    if (n > cache_len) {
        size_t needed = n;
        if (needed > cache_cap) {
            /* Increase capacity to at least needed. */
            cache_cap = needed;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        /* Fill new entries. */
        for (size_t i = cache_len; i < n; ++i) {
            idx_t idx = i + 1;  /* 1‑based index */
            if (idx == 1)        cache[i] = (seq_t)1;
            else if (idx == 2)   cache[i] = (seq_t)2;
            else if (idx == 3)   cache[i] = (seq_t)3;
            else if (idx == 4)   cache[i] = (seq_t)7;
            else if (idx == 5)   cache[i] = (seq_t)20;
            else if (idx == 6)   cache[i] = (seq_t)67;
            else {
                /* Recurrence: 11*a(n-1) - 41*a(n-2) + 61*a(n-3) - 30*a(n-4) */
                seq_t a1 = cache[i-1];
                seq_t a2 = cache[i-2];
                seq_t a3 = cache[i-3];
                seq_t a4 = cache[i-4];
                cache[i] = 11 * a1 - 41 * a2 + 61 * a3 - 30 * a4;
            }
        }
        cache_len = n;
    }
    return cache[n-1];
}
