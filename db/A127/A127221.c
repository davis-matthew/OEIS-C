/*
 * A127221  a(n) = 2^n*pentanacci(n) or (2^n)*A023424(n-1).
 * https://oeis.org/A127221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 2*a(n-1)+4*a(n-2)+8*a(n-3)+16*a(n-4)+32*a(n-5) offset: 1
 *            a(1)=2, a(2)=12, a(3)=56, a(4)=240, a(5)=992
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 1‑based memoization.  cache[0] is unused. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* maximum index computed so far */

    if (n == 0) return 0;  /* undefined, but return 0 for safety */

    /* Return cached value if already known */
    if (cache_len >= n) return cache[n];

    /* Need to grow the cache to at least n */
    idx_t new_len = n;
    seq_t *tmp = (seq_t *)realloc(cache, (new_len + 1) * sizeof(seq_t));
    if (!tmp) return 0;            /* allocation failed → sentinel value */
    cache = tmp;

    /* Ensure initial 5 terms are set (they may already be there) */
    if (new_len >= 1) cache[1] = (seq_t)2;
    if (new_len >= 2) cache[2] = (seq_t)12;
    if (new_len >= 3) cache[3] = (seq_t)56;
    if (new_len >= 4) cache[4] = (seq_t)240;
    if (new_len >= 5) cache[5] = (seq_t)992;

    /* Compute any missing terms using the recurrence */
    for (idx_t i = cache_len + 1; i <= new_len; ++i) {
        if (i <= 5) continue;  /* initial terms already set */
        cache[i] = (seq_t)2 * cache[i - 1]
                 + (seq_t)4 * cache[i - 2]
                 + (seq_t)8 * cache[i - 3]
                 + (seq_t)16 * cache[i - 4]
                 + (seq_t)32 * cache[i - 5];
    }

    cache_len = new_len;
    return cache[n];
}
