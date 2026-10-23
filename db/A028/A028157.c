/*
 * A028157  Expansion of 1/((1-4*x)*(1-8*x)*(1-9*x)*(1-12*x)).
 * https://oeis.org/A028157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=33, a(2)=697, a(3)=12033, a(n) = 33*a(n-1) -
 *            392*a(n-2) + 1968*a(n-3) - 3456*a(n-4) (n≥4)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored so far */
    static size_t cache_cap = 0;   /* capacity of the cache array */

    /* return immediately if already computed */
    if (n < cache_len) {
        return cache[n];
    }

    /* grow the cache if necessary */
    size_t need = (size_t)(n + 1);
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < need) new_cap <<= 1;  /* power‑of‑two growth */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first four terms if the cache is empty */
    if (cache_len == 0) {
        cache[0] = 1;
        cache[1] = 33;
        cache[2] = 697;
        cache[3] = 12033;
        cache_len = 4;
    }

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i < 4) {          /* already initialised */
            continue;
        }
        cache[i] =
            ((seq_t)33    * cache[i - 1])
          - ((seq_t)392   * cache[i - 2])
          + ((seq_t)1968  * cache[i - 3])
          - ((seq_t)3456  * cache[i - 4]);
    }

    cache_len = need;
    return cache[n];
}
