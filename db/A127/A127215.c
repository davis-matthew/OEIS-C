/*
 * A127215  a(n) = 3^n*tribonacci(n) or (3^n)*A001644(n+1).
 * https://oeis.org/A127215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 9*a(n-2) + 27*a(n-3), with a(1)=3, a(2)=27,
 *            a(3)=189
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* cache holds computed values, a(1) is at cache[0] */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    if (n < 1)
        return 0;                  /* behaviour below offset is irrelevant */

    /*--- initialize base terms on first call --------------------------------*/
    if (cache_len < 3) {
        size_t init_cap = 16;
        seq_t *tmp = seq_alloc_array(init_cap);
        if (!tmp) return 0;         /* allocation failure */
        tmp[0] = (seq_t)3;          /* a(1) */
        tmp[1] = (seq_t)27;         /* a(2) */
        tmp[2] = (seq_t)189;        /* a(3) */
        cache_len = 3;
        cache_cap = init_cap;
        if (cache) seq_free_array(cache);
        cache = tmp;
    }

    size_t m = (size_t)n;   /* n as size_t (n >= 1) */

    /*--- grow cache if requested index exceeds current capacity ----------*/
    if (m > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < m) new_cap = m;
        seq_t *new_arr = seq_alloc_array(new_cap);
        if (!new_arr) return 0;
        for (size_t i = 0; i < cache_len; ++i) new_arr[i] = cache[i];
        if (cache) seq_free_array(cache);
        cache = new_arr;
        cache_cap = new_cap;
    }

    /*--- return already computed value if present ------------------------*/
    if (m <= cache_len)
        return cache[m - 1];

    /*--- compute missing terms using the linear recurrence --------------*/
    for (size_t i = cache_len; i < m; ++i) {
        cache[i] = 3 * cache[i - 1] + 9 * cache[i - 2] + 27 * cache[i - 3];
    }
    cache_len = m;
    return cache[m - 1];
}
