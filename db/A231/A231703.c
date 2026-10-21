/*
 * A231703  Number of (n+1) X (1+1) 0..1 arrays with no element having a
 *          strict majority of its horizontal, vertical and antidiagonal
 *          neighbors equal to one.
 * https://oeis.org/A231703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A231703 recurrence implementation
   a(n) = 3*a(n-1) - a(n-2) + 4*a(n-3) - 8*a(n-4) + 3*a(n-5)
          - 3*a(n-6) + 3*a(n-7) - 2*a(n-8) + a(n-9)
   Offset: 1 (a(1) = 5).  The first nine values are hard‑coded
   to start the recurrence; no brute‑force search is performed.  */
#include <stdlib.h>

/* cache for already computed terms (a(1) stored in cache[0]) */
static seq_t *cache = NULL;
static idx_t cache_len = 0;  /* number of terms stored (a(1)...a(cache_len)) */
static idx_t cache_cap = 0;  /* capacity of the cache array          */

seq_t a(idx_t n)
{
    /* undefined for n == 0; return 0 safely */
    if (n == 0) return 0;

    /* lazy initialization: first nine terms of the sequence */
    if (cache_len == 0) {
        cache_cap = 16;                              /* ensure room for 9 terms */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                        /* memory allocation failed */

        cache[0] = 5;
        cache[1] = 17;
        cache[2] = 42;
        cache[3] = 121;
        cache[4] = 351;
        cache[5] = 978;
        cache[6] = 2768;
        cache[7] = 7851;
        cache[8] = 22168;
        cache_len = 9;
    }

    /* grow the cache if necessary */
    if (n > cache_cap) {
        idx_t newcap = cache_cap;
        while (newcap < n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;            /* memory reallocation failed */
        cache = tmp;
        cache_cap = newcap;
    }

    /* compute missing terms up to index n */
    if (n > cache_len) {
        for (idx_t i = cache_len; i < n; ++i) {
            /* recurrence uses the previous nine terms */
            seq_t val =
                  (seq_t)3 * cache[i-1]
                - cache[i-2]
                + (seq_t)4 * cache[i-3]
                - (seq_t)8 * cache[i-4]
                + (seq_t)3 * cache[i-5]
                - (seq_t)3 * cache[i-6]
                + (seq_t)3 * cache[i-7]
                - (seq_t)2 * cache[i-8]
                + cache[i-9];
            cache[i] = val;
        }
        cache_len = n;
    }

    return cache[n-1];
}
