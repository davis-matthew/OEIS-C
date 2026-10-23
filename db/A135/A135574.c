/*
 * A135574  A024495 but with terms swapped in pairs.
 * https://oeis.org/A135574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0)=0, a(1)=0, a(2)=3, a(3)=1, a(4)=11, a(5)=6
   a(n)=3*a(n-2)+3*a(n-4)+4*a(n-6)   (n>=6)    */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    if (n < cache_len)                 /* already computed */
        return cache[n];

    /* allocate or enlarge cache */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        while ((size_t)n >= new_cap)
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* extend cache up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)          cache[i] = 0;
        else if (i == 1)     cache[i] = 0;
        else if (i == 2)     cache[i] = 3;
        else if (i == 3)     cache[i] = 1;
        else if (i == 4)     cache[i] = 11;
        else if (i == 5)     cache[i] = 6;
        else {
            seq_t val = 3 * cache[i-2];
            val += 3 * cache[i-4];
            val += 4 * cache[i-6];
            cache[i] = val;
        }
    }
    cache_len = n + 1;
    return cache[n];
}
