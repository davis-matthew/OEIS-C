/*
 * A051926  Number of independent sets of nodes in graph C_4 X P_n (n>2).
 * https://oeis.org/A051926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=7, a(2)=35, a(n)=5*a(n-1)+a(n-2)-a(n-3) for n>2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* length of cache array */

    /* Ensure cache can index up to n */
    if (n + 1 > cache_len) {
        size_t new_len = n + 1;
        if (new_len < 3) new_len = 3;   /* at least store first three values */

        if (cache == NULL) {
            cache = (seq_t *)malloc(new_len * sizeof(seq_t));
            if (!cache) return (seq_t)0; /* allocation failure, unlikely in tests */
            cache_len = new_len;
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)7;
            cache[2] = (seq_t)35;
        } else if (new_len > cache_len) {
            cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
            if (!cache) return (seq_t)0; /* allocation failure */
        }

        /* Compute new entries using the recurrence */
        for (size_t i = cache_len; i < new_len; ++i) {
            /* recurrence: 5*a(i-1)+a(i-2)-a(i-3) */
            cache[i] = (seq_t)5 * cache[i - 1] + cache[i - 2] - cache[i - 3];
        }
        cache_len = new_len;
    }
    return cache[(size_t)n];
}
