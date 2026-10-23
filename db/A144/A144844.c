/*
 * A144844  a(n) = ((2 + sqrt(2))^n - (2 - sqrt(2))^n)^2/8.
 * https://oeis.org/A144844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 14*a(n-1) - 28*a(n-2) + 8*a(n-3), a(0)=0, a(1)=1, a(2)=16 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* handle the first three terms explicitly (no memoization needed) */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)16;

    /* initialize cache on first call */
    if (cache == NULL) {
        cache_len = 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (cache == NULL) { /* allocation failed, return 0 as a safe fallback */
            return (seq_t)0;
        }
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)16;
    }

    /* extend cache up to index n if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (tmp == NULL) { /* reallocation failed, keep old cache and fall back */
            /* compute the requested term directly without growing cache */
            seq_t a_nm3 = cache[0];
            seq_t a_nm2 = cache[1];
            seq_t a_nm1 = cache[2];
            for (idx_t i = 3; i <= n; ++i) {
                seq_t val = 14 * a_nm1 - 28 * a_nm2 + 8 * a_nm3;
                a_nm3 = a_nm2;
                a_nm2 = a_nm1;
                a_nm1 = val;
            }
            return a_nm1;
        }
        cache = tmp;
        /* compute missing terms using the recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t term = 14 * cache[i - 1];
            term += 8 * cache[i - 3];
            term -= 28 * cache[i - 2];
            cache[i] = term;
        }
        cache_len = new_len;
    }

    return cache[n];
}
