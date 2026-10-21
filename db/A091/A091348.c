/*
 * A091348  a(n) = 24*5^n - 60*4^n + 50*3^n - 15*2^n + 1.
 * https://oeis.org/A091348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*5^n - 60*4^n + 50*3^n - 15*2^n + 1 Equivalent linear
 *            recurrence: a(0)=0, a(1)=1, a(2)=31, a(3)=391, a(4)=3451
 *            a(n)=15*a(n-1) - 85*a(n-2) + 225*a(n-3) - 274*a(n-4) +
 *            120*a(n-5) (n>=5)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of terms stored (valid indices 0..cache_size-1) */
    static idx_t cache_cap = 0;    /* allocated size */

    /* ensure cache large enough for n+1 terms */
    if(cache_size <= n) {
        /* capacity expansion */
        idx_t needed = n + 1;
        if(cache_cap < needed) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
            while(new_cap < needed) new_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(cache, (size_t)new_cap * sizeof(seq_t));
            if(!tmp) exit(1);
            cache = tmp;
            cache_cap = new_cap;
        }
        /* initialise base values if cache was just created */
        if(cache_size == 0) {
            cache[0] = (seq_t)0;
            cache[1] = (seq_t)1;
            cache[2] = (seq_t)31;
            cache[3] = (seq_t)391;
            cache[4] = (seq_t)3451;
            cache_size = 5;
        }
        /* compute missing terms using the recurrence */
        while(cache_size <= n) {
            idx_t k = cache_size;
            /* a(k) = 15*a(k-1) - 85*a(k-2) + 225*a(k-3) - 274*a(k-4) + 120*a(k-5) */
            seq_t val = cache[k-1];
            val *= 15;

            seq_t tmp = cache[k-2];
            tmp *= 85;
            val -= tmp;

            tmp = cache[k-3];
            tmp *= 225;
            val += tmp;

            tmp = cache[k-4];
            tmp *= 274;
            val -= tmp;

            tmp = cache[k-5];
            tmp *= 120;
            val += tmp;

            cache[cache_size++] = val;
        }
    }
    return cache[n];
}
