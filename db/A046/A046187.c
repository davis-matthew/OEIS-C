/*
 * A046187  Indices of pentagonal numbers which are also octagonal.
 * https://oeis.org/A046187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A046187: Indices of pentagonal numbers which are also octagonal
 *
 * Recurrence:  a(n) = 1154*a(n-2) - a(n-4) - 192   (n > 4)
 * Initial values:  a(1)=1, a(2)=11, a(3)=1025, a(4)=12507
 *
 * This implementation memoises the values in a static array so that
 * any order of calls is supported and each term is computed only once.
 * It works correctly for any seq_t that supports integer arithmetic
 * (including arbitrary‑precision classes).  The malloc/realloc calls
 * are cast to seq_t* to be valid in both C11 and C++17.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;      /* array holding a(1)..a(cache_len) */
    static idx_t  cache_len = 0;     /* highest n that has been computed */
    static size_t cache_cap = 0;     /* allocated capacity */

    /* initialise cache storage if first call */
    if (cache == NULL) {
        cache_cap = 64;                      /* start with a small block   */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();          /* allocation failure */
    }

    /* grow the cache if we need to compute terms beyond cache_len */
    if (n > cache_len) {
        /* ensure allocation can hold up to n terms */
        size_t needed = (size_t)n;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap : 4;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute new terms from cache_len+1 up to n */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t val;
            switch (i) {
                case 1: val = (seq_t)1;       break;
                case 2: val = (seq_t)11;      break;
                case 3: val = (seq_t)1025;    break;
                case 4: val = (seq_t)12507;   break;
                default:
                    /* recurrence: a(i) = 1154*a(i-2) - a(i-4) - 192 */
                    val = ((seq_t)1154) * cache[i - 3] - cache[i - 5] - (seq_t)192;
                    break;
            }
            cache[i - 1] = val;              /* store at zero‑based index */
        }
        cache_len = n;
    }

    return cache[n - 1];   /* cached result */
}
