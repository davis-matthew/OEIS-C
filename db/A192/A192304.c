/*
 * A192304  0-sequence of reduction of (2n-1) by x^2 -> x+1.
 * https://oeis.org/A192304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 3*a(n-3) + a(n-4) + a(n-5) with
 *            a(1)=1, a(2)=1, a(3)=6, a(4)=13, a(5)=31 (recurrence applies for
 *            n ≥ 6)
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* 1‑based array of computed terms */
static idx_t  cache_len = 0;         /* number of terms currently stored */
static size_t cache_cap = 0;         /* allocated capacity (≥ cache_len+1) */

seq_t a(idx_t n)
{
    if (n < 1) {                      /* offset is 1; return 0 for n<1 */
        return (seq_t)0;
    }

    /* Ensure cache is initialized with first five terms */
    if (cache_len == 0) {
        size_t init_cap = 6;          /* indices 0..5; index 0 unused */
        cache = (seq_t*)calloc(init_cap, sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)13;
        cache[5] = (seq_t)31;
        cache_len = 5;
        cache_cap = init_cap;
    }

    /* Resize cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { free(cache); exit(EXIT_FAILURE); }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return already computed term */
    if (n <= cache_len) return cache[n];

    /* Compute new terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t t = cache[i-1];
        t += cache[i-1];             /* 2*a(i-1) */
        t += cache[i-1];             /* 3*a(i-1) */
        t -= cache[i-2];
        t -= cache[i-3]; t -= cache[i-3]; t -= cache[i-3]; /* 3*a(i-3) */
        t += cache[i-4];
        t += cache[i-5];
        cache[i] = t;
    }
    cache_len = n;
    return cache[n];
}
