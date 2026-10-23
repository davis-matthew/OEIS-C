/*
 * A015474  q-Fibonacci numbers for q=3, scale a(n-1).
 * https://oeis.org/A015474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n-1) * a(n-1) + a(n-2) for n ≥ 2, a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* memoisation buffer */
    static seq_t *cache = NULL;
    static idx_t capacity = 0;      /* allocated array size          */
    static idx_t computed = 0;      /* number of values actually computed */
    static seq_t pow3_current = 1;  /* 3^(last_computed_index)       */

    /* make sure array can hold a(n) */
    if (capacity <= n) {
        idx_t new_cap = (n >= 1) ? n + 1 : 2;   /* at least two slots for a(0) and a(1) */
        seq_t *tmp;
        if (cache == NULL) {
            tmp = (seq_t *)calloc((size_t)new_cap, sizeof(seq_t));
        } else {
            tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
            memset(tmp + (size_t)capacity, 0,
                   (size_t)(new_cap - capacity) * sizeof(seq_t));
        }
        cache = tmp;
        capacity = new_cap;
    }

    /* if the requested value is already available, return it */
    if (n < computed)
        return cache[n];

    /* initialise base terms on first use */
    if (computed == 0) {
        cache[0] = 0;
        cache[1] = 1;              /* capacity is at least 2 */
        computed = 2;
        pow3_current = 3;          /* 3^1 after a[1] is known */
    }

    /* compute missing values up to n */
    for (idx_t i = computed; i <= n; ++i) {   /* i ≥ 2 */
        seq_t val = cache[i - 1] * pow3_current;
        val = val + cache[i - 2];
        cache[i] = val;
        pow3_current = pow3_current * 3;      /* next power of three  */
    }

    if (computed < n + 1)
        computed = n + 1;

    return cache[n];
}
