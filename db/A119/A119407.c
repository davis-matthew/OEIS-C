/*
 * A119407  Number of nonempty subsets of {1,2,...,n} with no gap of length
 *          greater than 4 (a set S has a gap of length d if a and b are in S
 *          but no x with a < x < b is in S, where b-a=d).
 * https://oeis.org/A119407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-5) + a(n-6) for n ≥ 7 Initial
 *            values (offset 1): a(1)=1, a(2)=3, a(3)=7, a(4)=15, a(5)=31,
 *            a(6)=62
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 67, u128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i) for i>=1, cache[0] unused (0) */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;       /* capacity of allocated array   */
    static idx_t cached_max = 0;      /* largest n computed so far     */

    /* initial allocation and base values */
    if (cache == NULL) {
        /* allocate enough space for indices 0..6 (0 unused) */
        cache_cap = 8;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 3;
        cache[3] = 7;
        cache[4] = 15;
        cache[5] = 31;
        cache[6] = 62;
        cached_max = 6;
    }

    /* expand array if necessary */
    if (n > cached_max) {
        if (n + 2 > cache_cap) {            /* +2 to keep room for index n */
            size_t new_cap = cache_cap;
            while (new_cap <= n + 1) new_cap <<= 1;   /* double until big enough */
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
        /* compute missing values using the recurrence */
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            /* recurrence: a(i) = 3*a(i-1) - 2*a(i-2) - a(i-5) + a(i-6) */
            seq_t val = 3 * cache[i - 1];
            val -= 2 * cache[i - 2];
            val -= cache[i - 5];
            val += cache[i - 6];
            cache[i] = val;
        }
        cached_max = n;
    }

    return cache[n];
}
