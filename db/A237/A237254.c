/*
 * A237254  Values of x in the solutions to x^2 - 5xy + y^2 + 5 = 0, where 0 <
 *          x < y.
 * https://oeis.org/A237254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-2) - a(n-4) with initial values a(1)=1, a(2)=2,
 *            a(3)=3, a(4)=9
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 57, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* handle initial terms directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)9;

    /* static cache for previously computed terms */
    static seq_t *cache = NULL;          /* array indexed from 1 */
    static idx_t cache_cap = 0;          /* current allocated capacity (size + 1) */
    static idx_t computed_max = 0;       /* maximum n for which cache[n] is valid */

    /* ensure capacity for indices up to n */
    if (cache_cap < n + 1) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialize first four terms if not yet done */
    if (computed_max < 4) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)9;
        computed_max = 4;
    }

    /* compute any missing terms up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        cache[i] = ((seq_t)5 * cache[i - 2]) - cache[i - 4];
    }
    computed_max = n;

    return cache[n];
}
