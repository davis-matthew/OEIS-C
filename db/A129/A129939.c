/*
 * A129939  "Self-Lucas"; a(n) is the sum of the last 5 terms. Sequence starts
 *          with 12,21,3,1,19 which are l,u,c,a,s if you consider a=1, b=2,
 *          c=3, ..., z=26.
 * https://oeis.org/A129939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, word
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5) with a(1)=12, a(2)=21,
 *            a(3)=3, a(4)=1, a(5)=19
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 65, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* static cache of already computed terms (1‑based index) */
    static seq_t *cache      = NULL;  /* array of terms */
    static idx_t cache_len   = 0;      /* number of terms currently stored   */
    static idx_t cache_cap   = 0;      /* allocated size (array entries)     */

    /* initialise cache with the first five terms on first call */
    if (cache == NULL) {
        cache_cap = 6; /* indices 1..5 used, leave index 0 unused */
        cache = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        cache[1] = (seq_t)12;
        cache[2] = (seq_t)21;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)1;
        cache[5] = (seq_t)19;
        cache_len = 5;
    }

    /* if requested term is beyond current cache, extend cache */
    if (n > cache_len) {
        /* ensure the array is large enough (grow factor 2 for amortised O(1)) */
        if ((idx_t)(cache_cap) <= n) {
            idx_t new_cap = (n + 1) * 2;
            seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
            if (tmp == NULL) { /* allocation failed – cannot proceed */
                /* In a real library we might handle this error */
                return (seq_t)0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }
        /* compute missing terms */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* sum of the last five terms */
            cache[i] = cache[i - 1] + cache[i - 2] + cache[i - 3] +
                       cache[i - 4] + cache[i - 5];
        }
        cache_len = n;
    }

    return cache[n];
}
