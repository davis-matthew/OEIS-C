/*
 * A094451  a(n) = A033485(n) modulo 3.
 * https://oeis.org/A094451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1, a(n) = (a(n-1) + a(floor(n/2))) mod 3 (n ≥ 2)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;   /* array of computed values (1‑based) */
    static size_t cap   = 0;      /* current capacity of cache */
    static idx_t   last = 0;      /* highest index computed so far */

    /* grow the cache if required */
    if (n > last) {
        size_t needed = (size_t)n + 1;  /* need indices up to n (0 unused) */
        if (needed > cap) {
            size_t new_cap = (cap > 0) ? cap * 2 : 1;
            while (new_cap < needed) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();   /* out of memory */
            cache = tmp;
            cap   = new_cap;
        }
        /* compute missing values sequentially */
        for (idx_t i = last + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = 1;
            } else {
                cache[i] = (cache[i - 1] + cache[i / 2]) % (seq_t)3;
            }
        }
        last = n;
    }
    return cache[n];
}
