/*
 * A037660  Base 4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,1,0.
 * https://oeis.org/A037660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 4*a(n-1) + a(n-3) - 4*a(n-4)
 * Initial values: a(1)=3, a(2)=13, a(3)=52, a(4)=211
 * Computation is memoised so that calls in any order are efficient.
 */
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* initialise cache on first call */
    if (!cache) {
        cache_cap = (n > 4 ? n : 4);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;         /* allocation failed – return 0 */
        /* seed initial terms */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)13;
        cache[2] = (seq_t)52;
        cache[3] = (seq_t)211;
        cache_len = 4;
    }

    /* grow cache if requested index is beyond current capacity */
    if (n > cache_cap) {
        cache_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* reallocation failed */
        cache = tmp;
    }

    /* compute terms up to n using the recurrence */
    while (cache_len < n) {
        idx_t i = cache_len + 1;                /* current index (1‑based) */
        /* a(i) = 4*a(i-1) + a(i-3) - 4*a(i-4) */
        seq_t val = (seq_t)4 * cache[i-2]         /* 4*a(i-1) */
                  + cache[i-4]                    /* + a(i-3) */
                  - (seq_t)4 * cache[i-5];        /* - 4*a(i-4) */
        cache[cache_len++] = val;
    }

    return cache[n-1];
}
