/*
 * A024494  a(n) = C(n,1) + C(n,4) + ... + C(n, 3*floor(n/3) + 1).
 * https://oeis.org/A024494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=2, a(3)=3,
   a(n)=3*a(n-1)-3*a(n-2)+2*a(n-3)  for n≥4  (OEIS A024494) */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache = NULL;      /* store a(1)...a(cache_len) */
static idx_t cache_len = 0;      /* last computed index in cache */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined for 0, safe fallback */

    /* initialise with the first three terms */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)malloc((size_t)cache_len * sizeof(seq_t));
        if (cache == NULL) return 0;            /* allocation failure */
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)2;   /* a(2) */
        cache[2] = (seq_t)3;   /* a(3) */
    }

    /* extend the cache up to n, computing with the recurrence */
    if (n > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache,
                          (size_t)n * sizeof(seq_t));
        if (tmp == NULL) return 0;            /* allocation failure */
        cache = tmp;
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t v =
                (seq_t)3 * cache[(size_t)(i - 2)]   /* a(i-1) */
              - (seq_t)3 * cache[(size_t)(i - 3)]   /* a(i-2) */
              + (seq_t)2 * cache[(size_t)(i - 4)]; /* a(i-3) */
            cache[(size_t)(i - 1)] = v; /* store a(i) */
        }
        cache_len = n;
    }

    return cache[(size_t)(n - 1)]; /* a(n) */
}
