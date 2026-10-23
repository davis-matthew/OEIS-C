/*
 * A170614  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   The generating function is t^48+2 t^47+⋯+2 t+1
 *            -------------------------------- 378 t^48-27 t^47-⋯-27 t+1 From
 *            this one derives the linear recurrence a(n) =
 *            27⋅(a(n-1)+⋯+a(n-47)) – 378⋅a(n-48) + p(n) where p(0)=1, p(n)=2
 *            for 1≤n≤47, p(48)=1 and p(n)=0 otherwise. Initial condition:
 *            a(0)=1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* allocated size */
    static idx_t computed = 0;     /* number of terms stored (cache[0..computed-1]) */

    /* ensure cache can hold up to n */
    if (cache == NULL) {
        cache_size = (n < 64 ? 64 : n + 1);
        cache = alloc_seq_array(cache_size);
        /* zero initialise the array */
        for (idx_t i = 0; i < cache_size; ++i) cache[i] = 0;
        computed = 0;
    } else if (n + 1 > cache_size) {
        /* enlarge cache (double) */
        idx_t new_size = cache_size;
        while (n + 1 > new_size) new_size <<= 1;
        seq_t *new_cache = alloc_seq_array(new_size);
        for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
        for (idx_t i = cache_size; i < new_size; ++i) new_cache[i] = 0;
        free_seq_array(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    /* compute missing terms up to n */
    for (idx_t k = computed; k <= n; ++k) {
        seq_t p;
        if (k == 0)                p = 1;
        else if (k <= 47)          p = 2;
        else if (k == 48)          p = 1;
        else                       p = 0;

        seq_t sum = p;

        /* add 27 * a(k-i) for i = 1..47 */
        for (int i = 1; i <= 47; ++i) {
            if (k >= (idx_t)i) {
                sum += 27 * cache[k - (idx_t)i];
            }
        }

        /* subtract 378 * a(k-48) if applicable */
        if (k >= 48) {
            sum -= 378 * cache[k - 48];
        }

        cache[k] = sum;
    }

    computed = n + 1;
    return cache[n];
}
