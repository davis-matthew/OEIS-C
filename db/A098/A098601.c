/*
 * A098601  Expansion of (1+2*x)/((1+x)*(1-x^2-x^3)).
 * https://oeis.org/A098601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 41, u32 n <= 80, u64 n <= 159,
 *            u128 n <= 317, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A098601
   Recurrence:   a(n) = -a(n-1) + a(n-2) + 2*a(n-3) + a(n-4)
   Initial values: a(0)=1, a(1)=1, a(2)=0, a(3)=3
   This implementation memoises computed terms and extends the table on demand. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Fast path for the first four indices */
    if ((idx_t)n < 4) {
        static const seq_t first[4] = {1, 1, 0, 3};
        return first[(size_t)n];
    }

    size_t N = (size_t)n;             /* index as size_t for array access */

    /* Static cache for previously computed values */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;     /* allocated size (elements) */
    static size_t cache_len  = 0;     /* highest computed index */

    /* Allocate or grow cache as needed */
    if (cache == NULL) {
        cache_cap = 16;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
        /* initialise first four values */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 0;
        cache[3] = 3;
        cache_len = 3;
    } else if (cache_cap <= N) {
        while (cache_cap <= N) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            cache = tmp;
            cache_cap = new_cap;
        }
    }

    /* Compute all terms up to N, if not already done */
    for (size_t i = cache_len + 1; i <= N; ++i) {
        seq_t term_a = cache[i - 2];
        seq_t term_b = cache[i - 3] + cache[i - 3];  /* 2 * a(i-3) */
        seq_t term_c = cache[i - 4];
        seq_t sum = term_a + term_b + term_c;
        seq_t result = sum - cache[i - 1];
        cache[i] = result;
    }
    cache_len = N;

    return cache[N];
}
