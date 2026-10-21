/*
 * A088163  Numbers for which rotating one binary place to the right less
 *          rotating one binary place to the left is equal to zero.
 * https://oeis.org/A088163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-3) - 4*a(n-6) for n>5, with initial values a(0)=0,
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=7, a(5)=10
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 48, u64 n <= 96, u128 n <= 192,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache of already‑computed values */
    static seq_t *cache      = NULL;   /* cache array */
    static idx_t cache_len   = 0;      /* allocated length */
    static idx_t last_computed = 5;    /* highest index computed */

    /* initialise the cache the first time */
    if (cache_len == 0) {
        size_t init_len = 6;                           /* indices 0..5 */
        cache = (seq_t*)malloc(init_len * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure – unlikely in tests */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)7;
        cache[5] = (seq_t)10;
        cache_len = (idx_t)init_len;
    }

    /* enlarge the cache if needed */
    if (n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure */
        cache = tmp;
        cache_len = (idx_t)new_len;
    }

    /* compute missing terms up to n using the recurrence */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            /* recurrence: a(i) = 5*a(i-3) - 4*a(i-6) */
            cache[i] = (seq_t)5 * cache[i - 3] - (seq_t)4 * cache[i - 6];
        }
        last_computed = n;
    }

    return cache[n];
}
