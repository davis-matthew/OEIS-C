/*
 * A038797  T(n+4,n), array T as in A038792.
 * https://oeis.org/A038797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=6, a(2)=23, a(3)=73, a(4)=211, a(5)=581
   For n≥5: a(n)=6*a(n-1)-13*a(n-2)+13*a(n-3)-6*a(n-4)+a(n-5)  */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_capacity = 0;
    const seq_t init_vals[] = {1, 6, 23, 73, 211, 581};

    if (cache == NULL) {
        cache_capacity = 16;                 /* initial capacity */
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        for (idx_t i = 0; i < 6; ++i) cache[i] = init_vals[i];
        cache_size = 6;
    }

    if (n >= cache_size) {
        /* grow cache until it holds index n */
        if (n >= cache_capacity) {
            idx_t new_cap = cache_capacity;
            while (new_cap <= n) new_cap <<= 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_capacity = new_cap;
        }
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t v = 6 * cache[i - 1];
            v -= 13 * cache[i - 2];
            v += 13 * cache[i - 3];
            v -= 6 * cache[i - 4];
            v += cache[i - 5];
            cache[i] = v;
        }
        cache_size = n + 1;
    }
    return cache[n];
}
