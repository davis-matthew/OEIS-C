/*
 * A221677  Number of 0..2 arrays of length n with each element differing from
 *          at least one neighbor by 1 or less, starting with 0.
 * https://oeis.org/A221677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + a(n-3) Initial values: a(1) = 0,
 *            a(2) = 2, a(3) = 5
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 44, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache and its current capacity */
    static seq_t *cache = NULL;
    static idx_t   cached_len = 0;     /* largest index that has a value */
    static size_t  cur_cap    = 0;     /* allocated array size (>= cached_len+1) */

    /* the sequence is defined only for n >= 1 */
    if (n == 0) return 0;

    /* initialise cache with the first three terms if not yet done */
    if (cached_len < 3) {
        size_t init_cap = 4;  /* indices 0..3 are used */
#ifdef __cplusplus
        cache = new seq_t[init_cap];
#else
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
#endif
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;   /* a(1) */
        cache[2] = (seq_t)2;   /* a(2) */
        cache[3] = (seq_t)5;   /* a(3) */
        cached_len = 3;
        cur_cap = init_cap;
    }

    /* if the required index is already cached, return it */
    if (n <= cached_len) {
        return cache[n];
    }

    /* grow the cache if necessary (doubling strategy for efficiency) */
    if ((size_t)(n + 1) > cur_cap) {
        size_t new_cap = cur_cap ? cur_cap << 1 : 4;
        while (new_cap < (size_t)(n + 1)) {
            new_cap <<= 1;
        }
#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_cap];
        /* copy old data */
        for (idx_t i = 0; i <= cached_len; ++i) {
            tmp[i] = cache[i];
        }
        delete[] cache;
        cache = tmp;
#else
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
        cur_cap = new_cap;
    }

    /* compute new terms up to n using the recurrence */
    for (idx_t i = cached_len + 1; i <= n; ++i) {
        cache[i] = (seq_t)2 * cache[i-1] + (seq_t)2 * cache[i-2] + cache[i-3];
    }
    cached_len = n;

    return cache[n];
}
