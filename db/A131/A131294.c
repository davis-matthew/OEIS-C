/*
 * A131294  a(n)=ds_3(a(n-1))+ds_3(a(n-2)), a(0)=0, a(1)=1; where ds_3=digital
 *          sum base 3.
 * https://oeis.org/A131294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ds_3(a(n-1)) + ds_3(a(n-2)), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t capacity = 0;
    static idx_t max_computed = 0;

    /* initialisation on first call */
    if (cache == NULL) {
        capacity = 2;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        max_computed = 1;
    }

    /* extend cache if the requested index is beyond what is already known */
    if (n > max_computed) {
        idx_t new_cap = n + 1;
        if (new_cap > capacity) {
            size_t sz = (size_t)new_cap * sizeof(seq_t);
            seq_t *tmp = (seq_t *)realloc(cache, sz);
            if (!tmp) abort();          /* out of memory – terminate */
            cache = tmp;
            capacity = new_cap;
        }

        const seq_t three = (seq_t)3;
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t prev1 = cache[i - 1];
            seq_t prev2 = cache[i - 2];
            seq_t ds1 = (prev1 % three) + (prev1 / three);
            seq_t ds2 = (prev2 % three) + (prev2 / three);
            cache[i] = ds1 + ds2;       /* a(i) = ds_3(prev1) + ds_3(prev2) */
        }
        max_computed = n;
    }

    return cache[n];
}
