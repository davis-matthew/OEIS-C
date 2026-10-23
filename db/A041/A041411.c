/*
 * A041411  Denominators of continued fraction convergents to sqrt(220).
 * https://oeis.org/A041411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 178*a(n-4) - a(n-8) for n ≥ 8 initial terms: a(0)=1,
 *            a(1)=1, a(2)=5, a(3)=6, a(4)=173, a(5)=179, a(6)=889, a(7)=1068
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static size_t sz   = 0;   /* number of entries stored (≥ 0) */
    static size_t cap  = 0;   /* current allocation size (≥ sz) */

    /* initial values */
    static const seq_t init[] = {
        (seq_t)1, (seq_t)1, (seq_t)5, (seq_t)6,
        (seq_t)173, (seq_t)179, (seq_t)889, (seq_t)1068
    };
    const size_t init_len = sizeof(init) / sizeof(init[0]);

    /* for small indices return directly */
    if (n < init_len)
        return init[n];

    /* ensure cache has enough capacity for indices up to n */
    if (n + 1 > cap) {
        size_t new_cap = (n + 1) * 2;
        /* if this is the first allocation just use required size */
        if (cache == NULL)
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();   /* memory allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise cached values that are known (the first 8 terms) */
    if (sz < init_len) {
        for (size_t i = 0; i < init_len; ++i)
            cache[i] = init[i];
        sz = init_len;
    }

    /* compute values up to n using the recurrence */
    for (size_t i = sz; i <= (size_t)n; ++i) {
        /* a[i] = 178*a[i-4] - a[i-8] */
        cache[i] = (seq_t)178 * cache[i - 4] - cache[i - 8];
    }
    sz = (size_t)n + 1;   /* all values up to n are now cached */

    return cache[n];
}
