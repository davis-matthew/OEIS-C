/*
 * A163216  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A163216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 31*(a(n-1)+a(n-2)+a(n-3)-16*a(n-4)) Initial values:
 *            a(0)=1, a(1)=33, a(2)=1056, a(3)=33792, a(4)=1080816
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc */

seq_t a(idx_t n) {
    /* cached values of the sequence */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* initial values for n = 0..4 */
    const seq_t init[5] = { (seq_t)1, (seq_t)33, (seq_t)1056,
                            (seq_t)33792, (seq_t)1080816 };

    /* trivial cases */
    if (n < 5) return init[(size_t)n];

    /* initialise cache if needed */
    if (cache == NULL) {
        cache_cap = 8;                       /* start with some space */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) return 0;         /* allocation failure */
        for (size_t i = 0; i < 5; ++i) cache[i] = init[i];
        cache_len = 5;
    }

    /* extend cache if necessary */
    if (cache_len <= (size_t)n) {
        /* grow the allocation to at least n+1 elements */
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;          /* double */

        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (new_cache == NULL) return cache[cache_len-1];    /* keep old data */
        cache = new_cache;
        cache_cap = new_cap;

        /* compute missing values using the recurrence */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i < 5) {                       /* safety, shouldn't happen */
                cache[i] = init[i];
            } else {
                seq_t s = cache[i-1] + cache[i-2] + cache[i-3];
                seq_t t = cache[i-4] * (seq_t)16;
                cache[i] = (s - t) * (seq_t)31;
            }
        }
        cache_len = n + 1;
    }

    return cache[(size_t)n];
}
