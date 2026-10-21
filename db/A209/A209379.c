/*
 * A209379  1/4 the number of (n+1) X 6 0..2 arrays with every 2 X 2 subblock
 *          having distinct edge sums.
 * https://oeis.org/A209379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-2) - 68*a(n-4) + 80*a(n-6) for n ≥ 7 with initial
 *            values a(1)=64, a(2)=170, a(3)=320, a(4)=1076, a(5)=2176,
 *            a(6)=8648
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* current allocated length (index 0..len-1) */

    if (n == 0) return 0;  /* undefined, but not used */

    /* first call: allocate and initialize base terms */
    if (cache == NULL) {
        cache_len = 7;                 /* indices 0..6, 0 unused */
        cache = (seq_t*)calloc((size_t)cache_len, sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed (unlikely) */
        cache[1] = (seq_t)64;
        cache[2] = (seq_t)170;
        cache[3] = (seq_t)320;
        cache[4] = (seq_t)1076;
        cache[5] = (seq_t)2176;
        cache[6] = (seq_t)8648;
    }

    /* extend cache if necessary */
    if (n >= cache_len) {
        idx_t i;
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failed */
        cache = tmp;
        /* compute new values in increasing order to avoid uninitialized reads */
        for (i = cache_len; i <= n; ++i) {
            if (i <= 6) {
                /* already set, skip */
                continue;
            }
            /* recurrence: 16*a(i-2) - 68*a(i-4) + 80*a(i-6) */
            seq_t term1 = (seq_t)16 * cache[i-2];
            seq_t term2 = (seq_t)68 * cache[i-4];
            seq_t term3 = (seq_t)80 * cache[i-6];
            cache[i] = term1 - term2 + term3;
        }
        cache_len = new_len;
    }

    return cache[n];
}
