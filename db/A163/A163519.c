/*
 * A163519  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0)=1, a(1)=24, a(2)=552, a(3)=12696,
 * a(4)=292008, a(5)=6715908, and for n≥6
 * a(n)=22*(a(n-1)+a(n-2)+a(n-3)+a(n-4)) – 253*a(n-5).
 */
#include <stdlib.h> /* malloc, realloc */

seq_t a(idx_t n)
{
    /* memoization cache */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0; /* number of elements allocated   */
    static size_t cache_len = 0; /* indices [0..cache_len-1] have real values */

    size_t idx = (size_t)n;     /* eligible for array indexing */

    /* ensure enough capacity */
    if (idx >= cache_cap) {
        size_t new_cap = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out of memory: return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise base cases up to index 5 */
    if (cache_len <= 5) {
        const seq_t base_vals[6] = { (seq_t)1, (seq_t)24, (seq_t)552,
                                    (seq_t)12696, (seq_t)292008,
                                    (seq_t)6715908 };
        size_t i;
        for (i = cache_len; i < 6 && i < cache_cap; ++i)
            cache[i] = base_vals[i];
        cache_len = i > cache_len ? i : cache_len;
    }

    /* compute missing terms with the recurrence */
    if (cache_len <= idx) {
        size_t i;
        for (i = cache_len; i <= idx; ++i) {
            if (i < 6) continue; /* already initialised */
            seq_t sum4 = cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4];
            cache[i] = 22 * sum4 - 253 * cache[i-5];
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
