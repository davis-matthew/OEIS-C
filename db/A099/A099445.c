/*
 * A099445  An Alexander sequence for the Miller Institute knot.
 * https://oeis.org/A099445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=3, a(2)=6, a(3)=12, a(4)=25
 *            a(n)=3*a(n-1)-3*a(n-2)+3*a(n-3)-a(n-4) for n>4
 *            (a(n)=A099444(n)-A099444(n-2) also holds)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 57, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t  cache_cap = 0;   /* capacity of cache array */
    static size_t  cache_len = 0;   /* number of cached terms, i.e. last valid index + 1 */

    /* initialise cache with the first 5 terms */
    if (cache == NULL) {
        /* use a small initial capacity */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* out‑of‑memory: return 0 as a safe fallback */

        /* first five terms of the sequence */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)12;
        cache[4] = (seq_t)25;
        cache_len = 5;   /* indices 0..4 are cached */
    }

    /* enlarge cache if required */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        while ((size_t)n >= new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* out‑of‑memory: safe fallback */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute up to n if not already cached */
    for (idx_t i = cache_len; (idx_t)i <= n; ++i) {
        /* recurrence is defined for n > 4; cast i to idx_t for safety */
        seq_t v = 3*cache[i-1] - 3*cache[i-2] + 3*cache[i-3] - cache[i-4];
        cache[i] = v;
        ++cache_len;
    }

    return cache[n];
}
