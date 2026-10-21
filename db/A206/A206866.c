/*
 * A206866  Number of n X 3 0..1 arrays avoiding 0 0 0 horizontally and 1 0 1
 *          vertically.
 * https://oeis.org/A206866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 8*a(n-3) + 4*a(n-4) - 2*a(n-5) - a(n-6);
 *            Initial values: a(1)=7, a(2)=49, a(3)=211, a(4)=889, a(5)=3967,
 *            a(6)=17737
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
# include <new>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 */

    static seq_t *cache = NULL;
    static size_t cache_size = 0;  /* actual array length (index 0..cache_size-1) */
    static size_t computed = 6;    /* highest index computed so far */

    /* Ensure base cache of size at least 7 (indices 0..6) */
    if (cache_size < 7) {
        size_t old_size = cache_size;
        size_t new_size = 7; /* indices 0..6 inclusive */
        seq_t *new_cache;
#ifdef __cplusplus
        new_cache = new seq_t[new_size];
#else
        new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
#endif
        if (cache) {
#ifdef __cplusplus
            for (size_t i = 0; i < old_size; ++i) new_cache[i] = cache[i];
            delete[] cache;
#else
            memcpy(new_cache, cache, old_size * sizeof(seq_t));
            free(cache);
#endif
        }
        cache = new_cache;
        cache_size = new_size;

        /* initialise data for indices 1..6 */
        cache[0] = (seq_t)0;       /* unused */
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)49;
        cache[3] = (seq_t)211;
        cache[4] = (seq_t)889;
        cache[5] = (seq_t)3967;
        cache[6] = (seq_t)17737;
    }

    size_t idx = (size_t)n;

    /* Extend allocation if required */
    if (idx >= cache_size) {
        size_t old_size = cache_size;
        size_t new_size = idx + 1;
        seq_t *new_cache;
#ifdef __cplusplus
        new_cache = new seq_t[new_size];
#else
        new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
#endif
        if (cache) {
#ifdef __cplusplus
            for (size_t i = 0; i < old_size; ++i) new_cache[i] = cache[i];
            delete[] cache;
#else
            memcpy(new_cache, cache, old_size * sizeof(seq_t));
            free(cache);
#endif
        }
        /* initialise new elements to 0 (safe for arithmetic types or trivial constructors) */
        for (size_t i = old_size; i < new_size; ++i) new_cache[i] = (seq_t)0;
        cache = new_cache;
        cache_size = new_size;
    }

    /* Compute missing entries up to n (if any) */
    if (idx > computed) {
        for (size_t k = computed + 1; k <= idx; ++k) {
            seq_t val = 0;
            val += (seq_t)4 * cache[k - 1];
            val += (seq_t)8 * cache[k - 3];
            val += (seq_t)4 * cache[k - 4];
            val -= (seq_t)2 * cache[k - 5];
            val -= cache[k - 6];
            cache[k] = val;
        }
        computed = idx;
    }

    return cache[idx];
}
