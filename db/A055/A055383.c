/*
 * A055383  Expansion of
 *          (1+3*x+9*x^2+12*x^3+11*x^4+3*x^5+x^6)/((1-x)^2*(1-x^2)^2*(1-x^3)).
 * https://oeis.org/A055383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 24, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A055383 – Expansion of (1+3*x+9*x^2+12*x^3+11*x^4+3*x^5+x^6)
   /((1-x)^2*(1-x^2)^2*(1-x^3))
   The sequence satisfies the linear recurrence
       a(0)=1, a(1)=5, a(2)=20, a(3)=54, a(4)=123, a(5)=239,
       a(6)=428, a(7)=706, a(8)=1107
   and for n ≥ 9
       a(n) = 2*a(n-1) + a(n-2) - 3*a(n-3) - a(n-4)
              + a(n-5) + 3*a(n-6) - a(n-7) - 2*a(n-8) + a(n-9) */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;

    /* initialise cache with the first 9 terms if needed */
    if (cached_len == 0) {
        cached_len = 9;
        cache = (seq_t*)calloc(cached_len, sizeof(seq_t));
        if (!cache) return 0;                       /* allocation failed */
        cache[0] = 1;   cache[1] = 5;   cache[2] = 20;
        cache[3] = 54;  cache[4] = 123; cache[5] = 239;
        cache[6] = 428; cache[7] = 706; cache[8] = 1107;
    }

    /* if the requested index is already in the cache, return it */
    if (n < cached_len) return cache[n];

    /* grow the cache to accommodate up to index n */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return 0;                         /* allocation failed */
    cache = tmp;

    /* compute the missing terms using the recurrence */
    for (idx_t i = cached_len; i <= n; ++i) {
        cache[i] =
            2*cache[i-1] + cache[i-2]
          - 3*cache[i-3] - cache[i-4]
          + cache[i-5] + 3*cache[i-6]
          - cache[i-7] - 2*cache[i-8] + cache[i-9];
    }
    cached_len = new_len;
    return cache[n];
}
