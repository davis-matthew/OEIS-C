/*
 * A162783  Number of reduced words of length n in Coxeter group on 14
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) + 12*a(n-2) - 78*a(n-3) with a(0)=1, a(1)=14,
 *            a(2)=182, a(3)=2275
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_capacity = 0;
    static idx_t cache_size = 0;   /* number of elements already computed */

    /* initialise cache on first use */
    if (cache == NULL) {
        cache_capacity = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_capacity);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)14;
        cache[2] = (seq_t)182;
        cache[3] = (seq_t)2275;
        cache_size = 4;
    }

    /* grow cache to at least n+1 elements and compute missing recurrences */
    while (cache_size <= n) {
        /* ensure capacity */
        if (cache_size >= cache_capacity) {
            idx_t new_cap = cache_capacity * 2;
            if (new_cap == 0) new_cap = 4;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            cache = tmp;
            cache_capacity = new_cap;
        }

        /* compute a[cache_size] */
        idx_t i = cache_size;
        seq_t sum12 = (cache[i - 1] + cache[i - 2]) * (seq_t)12;
        seq_t diff78 = cache[i - 3] * (seq_t)78;
        cache[i] = sum12 - diff78;

        cache_size++;
    }

    return cache[n];
}
