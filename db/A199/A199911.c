/*
 * A199911  Number of -n..n arrays x(0..3) of 4 elements with zero sum, and
 *          adjacent elements not equal modulo three (with -1 modulo 3 = 2).
 * https://oeis.org/A199911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-3) - 3*a(n-4) - 3*a(n-6) + 3*a(n-7) +
 *            a(n-9) - a(n-10) (Empirical recurrence from OEIS A199911)
 *            Initial values a(1)..a(10): a(1)=8, a(2)=24, a(3)=72, a(4)=152,
 *            a(5)=256, a(6)=448, a(7)=680, a(8)=952, a(9)=1384, a(10)=1848
 * Range:     u8 n <= 4, u16 n <= 34, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined; offset is 1 */

    /* return known values directly */
    if (n <= 10) {
        static const seq_t base[11] = {0,8,24,72,152,256,448,680,952,1384,1848};
        return base[n];
    }

    /* static cache to support arbitrary order of calls */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static const seq_t base_vals[11] = {0,8,24,72,152,256,448,680,952,1384,1848};

    size_t old_len = cache_len;
    if (old_len <= n) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation error */
        cache = tmp;
        if (new_len > cache_len)
            memset(cache + cache_len, 0, (new_len - cache_len) * sizeof(seq_t));
        if (old_len <= 10) {
            for (idx_t i = 1; i <= 10 && (size_t)i < new_len; ++i)
                if (cache[i] == 0) cache[i] = base_vals[i];
        }
        cache_len = new_len;
    }

    /* compute new values using the recurrence */
    for (idx_t i = (old_len > 10 ? old_len : 11); i <= n; ++i) {
        long long val = (long long)cache[i-1]
                      + 3LL * (long long)cache[i-3]
                      - 3LL * (long long)cache[i-4]
                      - 3LL * (long long)cache[i-6]
                      + 3LL * (long long)cache[i-7]
                      + (long long)cache[i-9]
                      - (long long)cache[i-10];
        cache[i] = (seq_t)val;
    }
    return cache[n];
}
