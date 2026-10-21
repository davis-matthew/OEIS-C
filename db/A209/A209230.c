/*
 * A209230  Number of set partitions of [n] that avoid 1231 and 1121.
 * https://oeis.org/A209230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A209230: number of set partitions of [n] that avoid 1231 and 1121
 * Recurrence (n ≥ 5):
 *   a(n) = 4*a(n-1) - 4*a(n-2) + 2*a(n-4)
 * Initial values:
 *   a(0)=1, a(1)=1, a(2)=2, a(3)=5, a(4)=13
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_computed = (idx_t)-1;   /* -1 means none computed yet */

    /* ensure cache large enough for index n */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();          /* memory allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute from last_computed + 1 up to n */
    for (idx_t idx = last_computed + 1; idx <= n; idx += 1) {
        seq_t val;
        if (idx == 0)        val = (seq_t)1;
        else if (idx == 1)   val = (seq_t)1;
        else if (idx == 2)   val = (seq_t)2;
        else if (idx == 3)   val = (seq_t)5;
        else if (idx == 4)   val = (seq_t)13;
        else {
            /* recurrence: a = 4*a(n-1) - 4*a(n-2) + 2*a(n-4) */
            seq_t t1 = cache[idx - 1] * (seq_t)4;
            seq_t t2 = cache[idx - 2] * (seq_t)4;
            seq_t t3 = cache[idx - 4] * (seq_t)2;
            val = t1 - t2 + t3;
        }
        cache[idx] = val;
    }
    last_computed = n;
    return cache[n];
}
