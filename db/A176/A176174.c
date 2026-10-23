/*
 * A176174  Starting with 1, multiply the n-th term by 10, then subtract the
 *          sum of all terms up to and including the n-th, to make the (n+1)th
 *          term.
 * https://oeis.org/A176174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=9, a(n)=10*a(n-1)-10*a(n-2) for n≥3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array storing computed terms */
    static size_t  cap   = 0;     /* current capacity of the array   */
    static idx_t   max_n = 0;     /* largest index computed so far   */

    /* Allocate and initialise on first call or when capacity is insufficient */
    if (!cache) {
        cap = (size_t)n + 1;
        if (cap < 4) cap = 4;                 /* room for a(1) and a(2) */
#ifdef __cplusplus
        cache = new seq_t[cap];               /* default‑constructs to 0  */
#else
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
#endif
        if (!cache) abort();
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)9;
        max_n   = 2;
    }
    else if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_cap];
        for (size_t i = 0; i < cap; ++i) tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
#else
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        memset(tmp + cap, 0, (new_cap - cap) * sizeof(seq_t));
        cache = tmp;
#endif
        cap = new_cap;
    }

    /* Compute missing terms up to n if necessary */
    if (n > max_n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            cache[i] = (seq_t)10 * cache[i-1] - (seq_t)10 * cache[i-2];
        }
        max_n = n;
    }

    return cache[(size_t)n];
}
