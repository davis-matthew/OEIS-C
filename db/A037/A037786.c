/*
 * A037786  Base 4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,2,0,1.
 * https://oeis.org/A037786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 4*a(n-1) + d[n], where the digit sequence d is periodic
 *            3,2,0,1 (with a(0)=0). This builds the base‑4 number formed by
 *            the first n digits
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t pattern[4] = {3, 2, 0, 1};

seq_t a(idx_t n) {
    /* Cache of computed values: cache[i] == a(i).  cache[0] = 0. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* maximum index currently stored */

    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));   /* at least one element */
        if (!cache) return (seq_t)0;              /* fault‑tolerant */
        cache[0] = 0;
        cache_len = 0;
    }

    if (n > cache_len) {
        /* Grow array to hold indices up to n */
        size_t new_len_plus_one = (size_t)n + 1;   /* include index 0 */
        seq_t *new_cache = (seq_t *)realloc(cache, new_len_plus_one * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;           /* fault‑tolerant */
        cache = new_cache;

        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t digit = pattern[(i - 1) & 3];      /* (i-1) % 4 */
            cache[i] = (cache[i - 1] << 2) + digit; /* 4*prev + digit */
        }
        cache_len = n;
    }

    return cache[n];
}
