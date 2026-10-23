/*
 * A011918  a(n) = A011916(n) + A011922(n) - 1.
 * https://oeis.org/A011918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) - 15*a(n-2) + a(n-3); a(1)=5, a(2)=76,
 *            a(3)=1065
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (cache == NULL) {
        cache_len = 3;
        cache = (seq_t*)malloc((cache_len + 1) * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed */
        cache[0] = 0;                  /* unused */
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)76;
        cache[3] = (seq_t)1065;
    }

    if (n <= cache_len)
        return cache[n];

    /* Extend the cache up to index n */
    idx_t new_len = n;
    seq_t *tmp = (seq_t*)realloc(cache, (new_len + 1) * sizeof(seq_t));
    if (!tmp)
        return 0;                      /* allocation failed */
    cache = tmp;

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t t1 = cache[i - 1] * (seq_t)15;
        seq_t t2 = cache[i - 2] * (seq_t)15;
        cache[i] = (t1 - t2) + cache[i - 3];
    }
    cache_len = n;
    return cache[n];
}
