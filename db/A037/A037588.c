/*
 * A037588  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,1.
 * https://oeis.org/A037588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(7*9^n/20)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=3, a(2)=28, a(3)=255,
 * a(n)=9*a(n-1)+a(n-2)-9*a(n-3)  (for n>3) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset is 1, but guard against n=0 */

    /* Static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* initialise cache with first 3 terms */
    if (cache == NULL) {
        cache_cap = 3;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;       /* out of memory */
        cache[0] = (seq_t)3;        /* a(1) */
        cache[1] = (seq_t)28;       /* a(2) */
        cache[2] = (seq_t)255;      /* a(3) */
        cache_len = 3;
    }

    /* if already computed, return it */
    if (n <= cache_len)
        return cache[n - 1];

    /* grow cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute forward using the recurrence */
    for (size_t cur = cache_len; cur < (size_t)n; ++cur) {
        seq_t a_nm1 = cache[cur - 1];
        seq_t a_nm2 = cache[cur - 2];
        seq_t a_nm3 = cache[cur - 3];
        /* recurrence: 9*a_nm1 + a_nm2 - 9*a_nm3 */
        cache[cur] = (seq_t)(9 * a_nm1 + a_nm2 - 9 * a_nm3);
    }
    cache_len = (size_t)n;
    return cache[n - 1];
}
