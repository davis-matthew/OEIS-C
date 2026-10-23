/*
 * A024434  Expansion of 1/((1-x)(1-6x)(1-10x)(1-11x)).
 * https://oeis.org/A024434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=28, a(2)=521, a(3)=8120,
 *            a(n)=28*a(n-1)-263*a(n-2)+896*a(n-3)-660*a(n-4) for n>3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* number of elements stored */

    /* initialise cache with first four terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (cache == NULL) { /* allocation failed – terminate */
            exit(1);
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)28;
        cache[2] = (seq_t)521;
        cache[3] = (seq_t)8120;
        cache_size = 4;
    }

    /* return cached value if already computed */
    if (n < cache_size) {
        return cache[n];
    }

    /* enlarge cache to accommodate n */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (tmp == NULL) {
        exit(1);                      /* allocation failed */
    }
    cache = tmp;

    /* compute missing values using recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val = (seq_t)28 * cache[i - 1]
                  - (seq_t)263 * cache[i - 2]
                  + (seq_t)896 * cache[i - 3]
                  - (seq_t)660 * cache[i - 4];
        cache[i] = val;
    }

    cache_size = n + 1;
    return cache[n];
}
