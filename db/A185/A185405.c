/*
 * A185405  1/7 the number of n X 2 0..6 arrays with every element equal to
 *          exactly one or two of its horizontal and vertical neighbors.
 * https://oeis.org/A185405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) + 23*a(n-2) - 72*a(n-3) - 144*a(n-4) -
 *            216*a(n-5), with a(1)=1, a(2)=13, a(3)=150, a(4)=1848,
 *            a(5)=22656
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size (in slots) */
    static idx_t computed = 0;     /* largest index already computed */

    if (n == 0) return (seq_t)0;   /* offset starts at 1, unused zero case */

    /* ensure cache can hold indices 0..n; also reserve space for the first
       five entries needed for the recurrence */
    size_t need = (size_t)n + 1;
    if (need < 6) need = 6;
    if (need > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure – very unlikely */
        cache = tmp;
        cache_cap = need;
    }

    /* initialise the first five terms on the first use */
    if (computed == 0) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)13;
        cache[3] = (seq_t)150;
        cache[4] = (seq_t)1848;
        cache[5] = (seq_t)22656;
        computed = 5;
    }

    /* if the requested index has already been computed, just return it */
    if (n <= computed) return cache[n];

    /* compute missing terms up to n using the linear recurrence */
    for (idx_t k = computed + 1; k <= n; ++k) {
        seq_t val = (seq_t)11 * cache[k-1] + (seq_t)23 * cache[k-2]
                  - (seq_t)72 * cache[k-3] - (seq_t)144 * cache[k-4]
                  - (seq_t)216 * cache[k-5];
        cache[k] = val;
    }
    computed = n;
    return cache[n];
}
