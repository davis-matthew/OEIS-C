/*
 * A163232  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A163232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 44*a(n-1)+44*a(n-2)+44*a(n-3)-990*a(n-4) for n>=5 Initial
 *            values: a(0)=1, a(1)=46, a(2)=2070, a(3)=93150, a(4)=4190715
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   dealloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/dealloc_array.h"

static seq_t* alloc_array(idx_t sz)
{
    return (seq_t*)malloc(sz * sizeof(seq_t));
}

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t* cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed_upto = 4;   /* highest index that has been computed */

    /* --- initialization or resizing --- */
    if (cache == NULL) {
        /* start with a size that fits the first required entry */
        cache_size = 8;
        while (cache_size <= n) cache_size <<= 1;
        cache = alloc_array(cache_size);
        if (!cache) abort();

        /* base values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)46;
        cache[2] = (seq_t)2070;
        cache[3] = (seq_t)93150;
        cache[4] = (seq_t)4190715;
        computed_upto = 4;
    } else if (n + 1 > cache_size) {
        /* grow the cache */
        idx_t new_size = cache_size;
        while (new_size <= n) new_size <<= 1;
        seq_t* tmp = alloc_array(new_size);
        if (!tmp) abort();
        for (idx_t i = 0; i < cache_size; ++i) tmp[i] = cache[i];
        dealloc_array(cache);
        cache = tmp;
        cache_size = new_size;
    }

    /* --- use already known values for n <= 4 --- */
    if (n <= 4) return cache[n];

    /* --- compute missing terms up to n --- */
    if (computed_upto < n) {
        for (idx_t i = computed_upto + 1; i <= n; ++i) {
            seq_t term = cache[i-1] * (seq_t)44;
            term += cache[i-2] * (seq_t)44;
            term += cache[i-3] * (seq_t)44;
            term -= cache[i-4] * (seq_t)990;
            cache[i] = term;
        }
        computed_upto = n;
    }

    return cache[n];
}
