/*
 * A192352  Constant term of the reduction of the polynomial
 *          p(n,x)=(1/2)((x+1)^n+(x-1)^n) by x^2->x+1.
 * https://oeis.org/A192352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) = 2*a(n-1) + 3*a(n-2)
 *            - 4*a(n-3) + a(n-4) (n ≥ 5) with initial terms a(1)=1, a(2)=0,
 *            a(3)=2, a(4)=1
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;   /* cache[k] stores a(k+1) */
    static idx_t cache_len = 0;   /* number of terms currently stored */

    if (n < 1) return 0;  /* undefined for n<1, return 0 as sentinel */

    /* initialise base terms if not yet present */
    if (cache_len < 4) {
        idx_t needed = 4;
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        cache = tmp;

        if (cache_len < 1) cache[0] = 1;   /* a(1) */
        if (cache_len < 2) cache[1] = 0;   /* a(2) */
        if (cache_len < 3) cache[2] = 2;   /* a(3) */
        if (cache_len < 4) cache[3] = 1;   /* a(4) */
        cache_len = 4;
    }

    /* extend cache up to required index n */
    if ((idx_t)n > cache_len) {
        idx_t old_len = cache_len;
        idx_t new_len = (idx_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        cache = tmp;

        /* compute missing terms using the recurrence */
        for (idx_t i = old_len + 1; i <= n; ++i) {
            /* terms a(i-1), a(i-2), a(i-3), a(i-4) are at cache[i-2]..cache[i-5] */
            seq_t term = 2 * cache[i - 2];           /* 2*a(i-1) */
            term += 3 * cache[i - 3];               /* +3*a(i-2) */
            term -= 4 * cache[i - 4];               /* -4*a(i-3) */
            term += cache[i - 5];                   /* +a(i-4) */
            cache[i - 1] = term;                    /* store a(i) */
        }
        cache_len = new_len;
    }

    return cache[n - 1];
}
