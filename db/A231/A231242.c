/*
 * A231242  Number of n X 4 0..3 arrays x(i,j) with each element horizontally,
 *          diagonally or antidiagonally next to at least one element with
 *          value (x(i,j)+1) mod 4, no adjacent elements equal, and upper left
 *          element zero.
 * https://oeis.org/A231242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=44, a(3)=464, a(4)=5992, a(5)=76136, a(6)=968824,
 *            a(7)=12322784; for n≥8: a(n) = 10*a(n-1) + 30*a(n-2) + 53*a(n-3)
 *            + 71*a(n-4) - a(n-5) + 6*a(n-6) - 8*a(n-7); (empirical
 *            recurrence)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n<1 */

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0; /* number of terms stored */
    static size_t cache_cap  = 0; /* allocated capacity */

    /* initial values for n = 1..7 (0‑based indices 0..6) */
    static const seq_t init[7] = {
        (seq_t)0ULL,
        (seq_t)44ULL,
        (seq_t)464ULL,
        (seq_t)5992ULL,
        (seq_t)76136ULL,
        (seq_t)968824ULL,
        (seq_t)12322784ULL
    };

    /* determine how many terms are needed */
    size_t need = (size_t)n;                 /* 1‑based count */
    if (need == 0) need = 7;                 /* safety for n < 1 */
    if (need < 7) need = 7;                  /* at least 7 for recurrence */

    /* allocate / enlarge cache if necessary */
    if (need > cache_cap) {
        size_t new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure – return 0 as error value */
            free(cache);
            cache = NULL;
            cache_cap = cache_size = 0;
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise cache on first call */
    if (cache_size == 0) {
        for (size_t i = 0; i < 7; ++i)
            cache[i] = init[i];
        cache_size = 7;
    }

    /* compute additional terms using the recurrence */
    while (cache_size < need) {
        size_t k = cache_size;   /* k is 0‑based index of term to compute
                                    => a(k+1) */
        /* recurrence applies for k >= 7 (i.e. a(8) and beyond) */
        seq_t val = cache[k-1] * (seq_t)10ULL
                  + cache[k-2] * (seq_t)30ULL
                  + cache[k-3] * (seq_t)53ULL
                  + cache[k-4] * (seq_t)71ULL
                  - cache[k-5]
                  + cache[k-6] * (seq_t)6ULL
                  - cache[k-7] * (seq_t)8ULL;
        cache[k] = val;
        ++cache_size;
    }

    /* return the requested term (1‑based index → 0‑based array) */
    return cache[n-1];
}
