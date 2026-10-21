/*
 * A063108  a(1) = 1; thereafter a(n+1) = a(n) + product of nonzero digits of
 *          a(n).
 * https://oeis.org/A063108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, nice, look
 * Formula:   a(1) = 1; thereafter a(n+1) = a(n) + product of nonzero digits
 *            of a(n)
 * Range:     u8 n <= 23, u16 n <= 165, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* safety, though n>=1 in OEIS */
    static seq_t *cache = NULL;             /* cache of computed terms */
    static idx_t cache_size = 0;            /* highest index stored so far */

    /* lazy allocation: at least enough for the requested n */
    if (cache == NULL) {
        cache = (seq_t *)malloc((n + 2) * sizeof(seq_t)); /* +2 to keep 1‑based */
        if (!cache) return (seq_t)0;          /* allocation failure: not expected */
        cache[0] = 0;                         /* unused */
        cache[1] = (seq_t)1;                  /* a(1) */
        cache_size = 1;
    }

    /* extend cache if we haven't computed up to n yet */
    if (n > cache_size) {
        size_t new_len = (size_t)n + 2;       /* +2 for 1‑based safety */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure: not expected */
        cache = tmp;
        /* compute missing terms */
        for (idx_t k = cache_size + 1; k <= n; ++k) {
            seq_t x = cache[k - 1];
            seq_t prod = 1;
            seq_t temp = x;
            while (temp != 0) {
                seq_t d = temp % 10;
                if (d != 0) prod *= d;
                temp /= 10;
            }
            cache[k] = cache[k - 1] + prod;
        }
        cache_size = n;
    }

    return cache[n];
}
