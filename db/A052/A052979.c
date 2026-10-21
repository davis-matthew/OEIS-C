/*
 * A052979  Expansion of (1-x)(1+x)/(1-2*x-3*x^2+2*x^4).
 * https://oeis.org/A052979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A052979 – Expansion of (1‑x)(1+x)/(1‑2*x‑3*x²+2*x⁴)
 *
 *  Recurrence (valid for n ≥ 0):
 *        a(0) = 1,  a(1) = 2,  a(2) = 6,  a(3) = 18
 *    a(n+4) = 2*a(n+3) + 3*a(n+2) – 2*a(n)
 *  The function caches previously computed terms so that calls in any order
 *  are efficient.  The algorithm runs in O(n) time per distinct n and
 *  uses only integer arithmetic; it works for any seq_t / idx_t
 *  configuration that the test harness uses.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* number of valid entries */
    static idx_t cache_cap = 0;  /* allocated capacity */

    /* grow cache if needed */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory – terminate */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) cache[i] = (seq_t)1;
        else if (i == 1) cache[i] = (seq_t)2;
        else if (i == 2) cache[i] = (seq_t)6;
        else if (i == 3) cache[i] = (seq_t)18;
        else
            cache[i] = 2 * cache[i - 1] + 3 * cache[i - 2] - 2 * cache[i - 4];
    }
    cache_len = n + 1;
    return cache[n];
}
